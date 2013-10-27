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

function facilities_view_room($form) {

	$form['list'] = array(
		'#type' => 'item',
		'#markup' => facilities_list_rooms()
	);
	return $form;
}
