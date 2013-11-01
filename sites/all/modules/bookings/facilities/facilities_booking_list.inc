<?php
require_once(drupal_get_path('module', 'facilities') . '/facilities_constants.php');
require_once(drupal_get_path('module', FACILITIES_MODULE_NAME) . '/facilities_api.inc');

function facilities_booking($form, &$form_state) {
	global $user;

	$form['name'] = array(
		'#type' => 'textfield',
		'#title' => 'Facility Name',
		'#size' => 50,
		'#autocomplete_path' => FACILITIES_MODULE_NAME . '/auto_complete_name'
	);
	$form['filter'] = array(
		'#type' => 'submit',
		'#value' => 'Search'
	);

	if (!empty($form_state['values']['name'])) {
		$name = $form_state['values']['name'];
	} else {
		$name = -1;
	}
	$form['booking'] = array(
		'#type' => 'item',
		'#markup' => facilities_render_booking_by_user_and_facility_name($name, $user->uid),
	);
	return $form;
}

function facilities_booking_submit($form, &$form_state) {
	$form_state['rebuild'] = true;
}