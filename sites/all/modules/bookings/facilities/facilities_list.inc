<?php
require_once(drupal_get_path('module', 'facilities') . '/facilities_constants.php');
require_once(drupal_get_path('module', FACILITIES_MODULE_NAME) . '/facilities_api.inc');

function facilities_view_bbq($form) {
	$form['basic'] = array(
		'#markup' => '<p>this is a list of velues of the type: bbq</p>'
	);
	return $form;
}

function facilities_view_sports($form) {
	$form['basic'] = array(
		'#markup' => '<p>this is a list of velues of the type: sports</p>'
	);
	return $form;
}

function facilities_view_room($form, &$form_state) {
	if (isset($form_state['values'])) {
		dpm($form_state['values']);
		$values = $form_state['values'];
	} else {
		$values = array();
	}

	$form['search'] = facilities_list_search_form_room();

	$form['list'] = array(
		'#type' => 'item',
		'#markup' => facilities_list_rooms($values)
	);
	return $form;
}

function facilities_view_room_validate($form, &$form_state) {
	
}

function facilities_view_room_submit($form, &$form_state) {
	$form_state['rebuild'] = true;
}
