<?php
require_once(drupal_get_path('module', 'facilities') . '/facilities_constants.php');
require_once(drupal_get_path('module', 'facilities') . '/facilities_api.inc');

function facilities_enrollment($form, &$form_state)  {
	if (is_enrolled()) {
		drupal_goto('facilities/view');
	}
	
	$form['uni'] = array(
		'#type' => 'select',
		'#title' => 'University',
		'#options' => array('NUS' => 'NUS', 'NTU' => 'NTU'),
		'#default_value' => ''
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => 'Confirm'
	);
	return $form;	
}

function facilities_enrollment_submit($form, &$form_state) {
	global $user;
	$uni = $form_state['values']['uni'];
	$qry = db_insert(UNI_TABLE)
		->fields(array(
			'user_id' => $user->uid,
			'university' => $uni
		));

	$qry->execute();
}