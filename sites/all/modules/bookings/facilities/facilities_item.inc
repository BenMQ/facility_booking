<?php
require_once(drupal_get_path('module', 'facilities') . '/facilities_constants.php');
require_once(drupal_get_path('module', FACILITIES_MODULE_NAME) . '/facilities_api.inc');

function facilities_view_item($form, &$form_state, $id) {
	$form['list'] = array(
		'#type' => 'item',
		'#markup' => facilities_render_by_id($id)
	);

	return $form;
}

function facilities_view_bbq_item($form, &$form_state, $id) {
	$form['list'] = array(
		'#type' => 'item',
		'#markup' => $id
	);
	
	return $form;
}

function facilities_view_room_item($form, &$form_state, $id) {
	$form['list'] = array(
		'#type' => 'item',
		'#markup' => $id
	);
	
	return $form;
}