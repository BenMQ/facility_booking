<?php
require_once(drupal_get_path('module', 'facilities') . '/facilities_constants.php');
require_once(drupal_get_path('module', FACILITIES_MODULE_NAME) . '/facilities_api.inc');

function facilities_view_bbq($form, &$form_state) {
	if (isset($form_state['values'])) {
		$values = $form_state['values'];
	} else {
		$values = array();
	}

	$form['search'] = facilities_list_search_form_bbq();

	$form['list'] = array(
		'#type' => 'item',
		'#markup' => facilities_list_bbq($values)
	);
	return $form;
}

function facilities_view_bbq_submit($form, &$form_state) {
	$form_state['rebuild'] = true;
}

function facilities_view_sports($form, &$form_state) {
	if (isset($form_state['values'])) {
		$values = $form_state['values'];
	} else {
		$values = array();
	}

	$form['search'] = facilities_list_search_form_sports();

	$form['list'] = array(
		'#type' => 'item',
		'#markup' => facilities_list_sports($values)
	);
	return $form;
}

function facilities_view_sports_submit($form, &$form_state) {
	$form_state['rebuild'] = true;
}

function facilities_view_room($form, &$form_state) {
	if (isset($form_state['values'])) {
		$values = $form_state['values'];
	} else {
		$values = array();
	}

	$form['search'] = facilities_list_search_form_room();

	$form['list'] = array(
		'#type' => 'item',
		'#markup' => facilities_list_room($values)
	);
	return $form;
}


function facilities_view_room_submit($form, &$form_state) {
	$form_state['rebuild'] = true;
}
