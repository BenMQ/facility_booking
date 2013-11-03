<?php
require_once(drupal_get_path('module', 'facilities') . '/facilities_constants.php');
require_once(drupal_get_path('module', FACILITIES_MODULE_NAME) . '/facilities_api.inc');
require_once(drupal_get_path('module', FACILITIES_MODULE_NAME) . '/facilities_item.inc');

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

function facilities_booking_edit($form, &$form_state, $booking_id) {
	global $user;

	$booking = facilities_get_booking_by_id($booking_id)->fetchObject();

	if ($booking && ($booking->user_id == $user->uid || is_admin())) {
		$form = facilities_view_item($form, $form_state, $booking->facility_id, $booking->date, $booking->start, $booking->end);
	} else {
		drupal_not_found();
		return;
	}
	$form['notice'] = array(
		'#type' => 'item',
		'#markup' => '<h3>You may book a facility 2 weeks in advance, with maximum 2 hours per booking and 1 booking per facility per day.</h3>'
	);
	$form['booking_id'] = array(
		'#type' => 'hidden',
		'#value' => $booking->id
	);
	$form['facility_id'] = array(
		'#type' => 'hidden',
		'#value' => $booking->facility_id
	);
	return $form;
}


function facilities_booking_edit_validate($form, &$form_state) {
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

function facilities_booking_edit_submit($form, &$form_state) {
	global $user;
	facilities_modify_booking($form_state['values']['booking_id'], $form_state['values']['facility_id'], $user->uid,
		$form_state['values']['date'], $form_state['values']['from'], $form_state['values']['to']);
}

function facilities_booking_delete($form, &$form_state, $booking_id) {
	global $user;
	$uid = db_select(BOOKING_TABLE, 'b')
		->condition('b.id', $booking_id)
		->fields('b', array('user_id'))
		->execute()
		->fetchField();
	if (!$uid || ($uid != $user->uid && !is_admin())) {
		drupal_access_denied();
		return;
	}
	$qry = db_delete(BOOKING_TABLE)
		->condition('id', $booking_id)
		->execute();
	drupal_set_message("Booking deleted!");
	drupal_goto('booking/view');
}