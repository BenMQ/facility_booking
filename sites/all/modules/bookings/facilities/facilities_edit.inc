<?php
require_once(drupal_get_path('module', 'facilities') . '/facilities_constants.php');
require_once(drupal_get_path('module', FACILITIES_MODULE_NAME) . '/facilities_api.inc');

function facilities_edit($form, $form_state, $id = 0, $type = null) {
	if ($id > 0) {
		$entry = facilities_get_by_id($id)->fetchObject();
		if ($type == null) {
			$type = facilities_get_type_by_id($id);
		}
	}
	$form['id'] = array(
		'#type' => 'hidden',
		'#value' => $id
	);
	$form['facility_type'] = array(
		'#type' => 'hidden',
		'#value' => $type
	);
	$form['name'] = array(
		'#type' => 'textfield',
		'#title' => 'Name: ',
		'#size' => 50,
		'#maxlength' => 100,
		'#default_value' => $id > 0 ? $entry->name : '',
		'#required' => true
	);

	$form['university'] = array(
		'#type' => 'textfield',
		'#title' => 'University: ',
		'#size' => 50,
		'#maxlength' => 100,
		'#default_value' => $id > 0 ? $entry->university : '',
		'#required' => true,
		'#autocomplete_path' => FACILITIES_MODULE_NAME . '/auto_complete_university'
	);

	$form['location'] = array(
		'#type' => 'textfield',
		'#title' => 'Location: ',
		'#size' => 50,
		'#maxlength' => 100,
		'#default_value' => $id > 0 ? $entry->location : '',
		'#required' => true,
		'#autocomplete_path' => FACILITIES_MODULE_NAME . '/auto_complete_location'
	);

	$form['indoor'] = array(
		'#type' => 'select',
		'#title' => 'Indoor / Outdoor: ',
		'#options' => array('0' => 'Outdoor', '1' => 'Indoor'),
		'#default_value' => $id > 0 ? $entry->indoor : '',
		'#required' => true
	);

	$form['capacity'] = array(
		'#type' => 'textfield',
		'#title' => 'Capacity: ',
		'#size' => 50,
		'#maxlength' => 4,
		'#default_value' => $id > 0 ? $entry->capacity : '',
		'#element_validate' => array('facilities_positive_int_field_validator'),
		'#required' => true
	);

	$form['cost'] = array(
		'#type' => 'textfield',
		'#title' => 'Cost per hour: ',
		'#size' => 50,
		'#maxlength' => 10,
		'#default_value' => $id > 0 ? $entry->cost : '0',
		'#element_validate' => array('facilities_positive_float_field_validator'),
		'#required' => true
	);

	switch ($type) {
		case BBQ:
			$form['sheltered'] = array(
				'#type' => 'select',
				'#title' => 'Sheltered',
				'#options' => array('1' => 'Yes', '0' => 'No'),
				'#default_value' => $id > 0 ? $entry->sheltered : '1',
				'#required' => true
			);

			$form['water_source'] = array(
				'#type' => 'select',
				'#title' => 'Has water source',
				'#options' => array('1' => 'Yes', '0' => 'No'),
				'#default_value' => $id > 0 ? $entry->water_source : '1',
				'#required' => true
			);

			$form['grill'] = array(
				'#type' => 'textfield',
				'#title' => 'Grills',
				'#size' => 50,
				'#maxlength' => 3,
				'#default_value' => $id > 0 ? $entry->grill : '',
				'#required' => true,
				'#element_vaidate' => array('facilities_positive_int_field_validator')
			);
			break;
		case ROOM:
			$form['projector'] = array(
				'#type' => 'textfield',
				'#title' => 'Projectors',
				'#size' => 50,
				'#maxlength' => 50,
				'#required' => true,
				'#element_validate' => array('facilities_positive_int_field_validator'),
				'#default_value' => $id > 0 ? $entry->projector : '',
			);

			$form['whiteboard'] = array(
				'#type' => 'textfield',
				'#title' => 'Whiteboards',
				'#size' => 50,
				'#maxlength' => 50,
				'#required' => true,
				'#element_validate' => array('facilities_positive_int_field_validator'),
				'#default_value' => $id > 0 ? $entry->whiteboard : '',
			);

			$form['power_source'] = array(
				'#type' => 'textfield',
				'#title' => 'Power Plugs',
				'#size' => 50,
				'#maxlength' => 50,
				'#required' => true,
				'#element_validate' => array('facilities_positive_int_field_validator'),
				'#default_value' => $id > 0 ? $entry->power_source : '',
			);
			break;
		case SPORTS:
			$form['type'] = array(
				'#type' => 'textfield',
				'#title' => 'Facility type',
				'#size' => 50,
				'#maxlength' => 50,
				'#default_value' => $id > 0 ? $entry->type : '',
				'#required' => true,
				'#autocomplete_path' => FACILITIES_MODULE_NAME . '/auto_complete_location'
			);
			break;
		
	}
	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => 'Save'
	);

	return $form;
}

function facilities_edit_submit($form, &$form_state) {
	$values = $form_state['values'];
	foreach($values as $key => $value) {
		$values[$key] = trim($value);
	}
	if ($values['id'] == 0) {
		$id = facilities_create_common($values);
	} else {
		$id = $values['id'];
		facilities_update_common($values, $id);
	}
	switch ($values['facility_type']) {
		case BBQ:
			facilities_update_bbq($values, $id);
			break;
		case ROOM:
			facilities_update_room($values, $id);
			break;
		case SPORTS:
			facilities_update_sports($values, $id);
			break;
	}

	drupal_set_message('Facility information has been saved.');

	if ($values['id'] == 0) {
		drupal_goto(FACILITIES_MODULE_NAME . '/edit/' . $id);
	}
}


function facilities_edit_new($form, &$form_state) {

}