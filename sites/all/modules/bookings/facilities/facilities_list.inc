<?php
require_once(drupal_get_path('module', 'facilities') . '/facilities_constants.php');

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
	$form['basic'] = array(
		'#markup' => '<p>this is a list of velues of the type: room</p>'
	);
	return $form;
}
