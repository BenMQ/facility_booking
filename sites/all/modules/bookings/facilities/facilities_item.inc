<?php
require_once(drupal_get_path('module', 'facilities') . '/facilities_constants.php');
require_once(drupal_get_path('module', FACILITIES_MODULE_NAME) . '/facilities_api.inc');

function facilities_view_item($form, &$form_state, $id) {
	$form['list'] = array(
		'#type' => 'item',
		'#markup' => facilities_render_by_id($id)
	);

	$form['id'] = array(
		'#type' => 'hidden',
		'#value' => $id,
	);

	$form['date'] = array(
		'#type' => 'date_popup',
		'#title' => t('Date'),
		'#date_format' => 'd-m-Y',
		'#default_value' => date('Y-m-d', strtotime('+1 day')),
		'#size' => 10,
		'#date_label_position' => 'none',
		'#required' => true,
	);

	$hours = array();
	for ($i=8; $i < 23; $i++) { 
		$hours[$i] = $i . ':00';
	}

	$form['from'] = array(
		'#type' => 'select',
		'#title' => 'From',
		'#options' => $hours,
		'#default_value' => '8',
		'#required' => true,
	);

	$form['to'] = array(
		'#type' => 'select',
		'#title' => 'To',
		'#options' => $hours,
		'#default_value' => '10',
		'#required' => true,
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => 'Book Now'
	);

	return $form;
}

function facilities_view_item_validate($form, &$form_state) {
	if ($form_state['values']['from'] >= $form_state['values']['to']) {
		form_error($form['to'], 'Invalid time range.');
	}
	if ($form_state['values']['to'] - $form_state['values']['from'] > 2) {
		form_error($form['to'], 'Maximum booking of 2 hours.');
	}
	if ($form_state['values']['date'] < date('Y-m-d')) {
		form_error($form['date'], 'Please do not select a day in the past');
	}
	if ($form_state['values']['date'] == date('Y-m-d') && $form_state['values']['from'] < date('h')) {
		form_error($form['from'], 'Please do not select a time in the past');
	}
	if ($form_state['values']['date'] == date('Y-m-d') && $form_state['values']['to'] < date('h')) {
		form_error($form['to'], 'Please do not select a time in the past');
	}
	if ($form_state['values']['date'] > date('Y-m-d', strtotime('+2 weeks'))) {
		form_error($form['date'], 'You can only place bookings at most 2 weeks in advance');
	}

}

function facilities_view_item_submit($form, &$form_state) {
	global $user;
	facilities_make_booking($form_state['values']['id'], $user->uid,
		$form_state['values']['date'], $form_state['values']['from'], $form_state['values']['to']);
}
