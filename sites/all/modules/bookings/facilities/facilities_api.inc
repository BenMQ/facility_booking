<?php
require_once(drupal_get_path('module', 'facilities') . '/facilities_constants.php');

/*
 *  ######   #######  ##     ## ##     ##  #######  ##    ## 
 * ##    ## ##     ## ###   ### ###   ### ##     ## ###   ## 
 * ##       ##     ## #### #### #### #### ##     ## ####  ## 
 * ##       ##     ## ## ### ## ## ### ## ##     ## ## ## ## 
 * ##       ##     ## ##     ## ##     ## ##     ## ##  #### 
 * ##    ## ##     ## ##     ## ##     ## ##     ## ##   ### 
 *  ######   #######  ##     ## ##     ##  #######  ##    ## 
 */  

function facilities_list_search_form_common($capacity_opt = array(), $cost_opt = array()) {
	$form = array(
		'#type' => 'fieldset',
		'#title' => t('Search'),
		'#collapsible' => TRUE,
		'#collapsed' => FALSE,
	);

	$form['location'] = array(
		'#type' => 'textfield',
		'#title' => t('Location'),
		'#size' => 50,
		'#autocomplete_path' => FACILITIES_MODULE_NAME . '/auto_complete_location'
	);

	$form['indoor'] = array(
		'#type' => 'select',
		'#title' => 'Indoor / outdoor',
		'#options' => array('' => 'Any', 'indoor' => 'Indoor', 'outdoor' => 'Outdoor'),
		'#default_value' => ''
	);

	if (empty($capacity_opt)) {
		$capacity_opt = array('' => 'Any', '0-20' => '< 20', '20-50' => '20 ~ 50',
							'50-100' => '50 ~ 100', '100-200' => '100 ~ 200', '200' => '> 200');
	}
	$form['capacity'] = array(
		'#type' => 'select',
		'#title' => 'Capacity',
		'#options' => $capacity_opt,
		'#default_value' => ''
	);

	if (empty($cost_opt)) {
		$cost_opt = array('' => 'Any', '0' => 'Free', '50' => '< 50',
							'100' => '< 100', '500' => '< 500');
	}
	$form['cost'] = array(
		'#type' => 'select',
		'#title' => 'Cost per hour',
		'#options' => $cost_opt,
		'#default_value' => ''
	);

	return $form;
}

function facilities_list_common_query($search, $table) {
	$qry = db_select(FACILITY_TABLE, 'f');
	$qry->join($table, 'special', 'special.facility_id = f.id');
	$qry->fields('f')
		->fields('special');


	if (isset($search['indoor']) && $search['indoor']) {
		$value = $search['indoor'] === 'outdoor' ? 0 : 1;
		$qry->condition('f.indoor', $value);
	}

	if (isset($search['capacity']) && $search['capacity']) {
		$range = explode('-', $search['capacity']);
		$range_int = array();
		$range_int[] = intval($range[0]);
		if (count($range) > 0) {
			$range_int[] = intval($range[1]);
			$qry->condition('f.capacity', $range_int);
		} else {
			$qry->condition('f.capacity', $range_int[0], '>');
		}
	}

	if (isset($search['cost']) && $search['cost']) {
		$cost = intval($search['cost']);
		$qry->condition('f.cost', $cost, '<');
	}

	if (isset($search['location']) && trim($search['location'])) {
		$qry->condition('f.location', '%'.trim($search['location']).'%', 'LIKE');
	}

	return $qry;
}

function facilities_get_by_id($id, $type = null) {
	if ($type == null) {
		$type = facilities_get_type_by_id($id);
	}
	if ($type == NOT_FOUND) {
		drupal_not_found();
	}
	$qry = db_select(FACILITY_TABLE, 'f');
	$qry->join(FACILITY_TABLE . '_' . $type, 'special', 'special.facility_id = f.id');
	$qry->condition('f.id', $id)
		->fields('f')
		->fields('special');
	$entry = $qry -> execute();
	
	return $entry;
}

function facilities_render_by_id($id, $type = null) {
	if ($type == null) {
		$type = facilities_get_type_by_id($id);
	}
	$entry = facilities_get_by_id($id);
	switch ($type) {
		case BBQ:
			return facilities_render_bbq($entry, false);
			break;
		case SPORTS:
			return facilities_render_sports($entry, false);
			break;
		case ROOM:
			return facilities_render_room($entry, false);
			break;
		default:
			drupal_not_found();
	}
}

function facilities_get_type_by_id($id) {
	$bbq = db_select(BBQ_TABLE, 's')
			->condition('s.facility_id', $id)
			->fields('s')
			->execute();
	if ($bbq->rowCount() > 0) {
		return BBQ;
	}

	$sports = db_select(SPORTS_TABLE, 's')
			->condition('s.facility_id', $id)
			->fields('s')
			->execute();
	if ($sports->rowCount() > 0) {
		return SPORTS;
	}

	$room = db_select(ROOM_TABLE, 's')
			->condition('s.facility_id', $id)
			->fields('s')
			->execute();
	if ($room->rowCount() > 0) {
		return ROOM;
	}

	return NOT_FOUND;
}

function facilities_create_common($values) {
	$qry = db_insert(FACILITY_TABLE)
		->fields(array(
			'name' => $values['name'],
			'location' => $values['location'],
			'university' => $values['university'],
			'indoor' => $values['indoor'],
			'capacity' => $values['capacity'],
			'cost' => $values['cost']
			));
	$res = $qry -> execute();
	return $res;
}

function facilities_update_common($values, $id) {
	$qry = db_update(FACILITY_TABLE)
		->fields(array(
			'name' => $values['name'],
			'location' => $values['location'],
			'university' => $values['university'],
			'indoor' => $values['indoor'],
			'capacity' => $values['capacity'],
			'cost' => $values['cost']
			))
		->condition('id', $id);
	$res = $qry -> execute();
	return $res;
}
/*
 * ########   #######   #######  ##     ## 
 * ##     ## ##     ## ##     ## ###   ### 
 * ##     ## ##     ## ##     ## #### #### 
 * ########  ##     ## ##     ## ## ### ## 
 * ##   ##   ##     ## ##     ## ##     ## 
 * ##    ##  ##     ## ##     ## ##     ## 
 * ##     ##  #######   #######  ##     ## 
 */

function facilities_list_search_form_room() {
	$form = facilities_list_search_form_common();

	$form['projector'] = array(
		'#type' => 'textfield',
		'#title' => 'Projectors Required',
		'#size' => 8,
		'#element_validate' => array('facilities_positive_int_field_validator')
	);

	$form['whiteboard'] = array(
		'#type' => 'textfield',
		'#title' => 'Whiteboards Required',
		'#size' => 8,
		'#element_validate' => array('facilities_positive_int_field_validator')
	);

	$form['power_source'] = array(
		'#type' => 'textfield',
		'#title' => 'Power Plugs Required',
		'#size' => 8,
		'#element_validate' => array('facilities_positive_int_field_validator')
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => 'Search'
	);

	return $form;
}


function facilities_list_room($search) {
	$qry = facilities_list_common_query($search, ROOM_TABLE);

	if (isset($search['projector']) && $search['projector']) {
		$required = intval($search['projector']);
		$qry->condition('special.projector', $required, '>=');
	}
	if (isset($search['whiteboard']) && $search['whiteboard']) {
		$required = intval($search['whiteboard']);
		$qry->condition('special.whiteboard', $required, '>=');
	}
	if (isset($search['power_source']) && $search['power_source']) {
		$required = intval($search['power_source']);
		$qry->condition('special.power_source', $required, '>=');
	}

	$res = $qry->execute();
	return facilities_render_room($res);
}

function facilities_render_room($entry, $with_action = true) {
	$header = array('Name', 'Location', 'Indoor', 'Capacity', 'Cost', 'Resources');
	if ($with_action) {
		$header[] = 'Actions';
	}
	$rows = array();
	foreach($entry as $room) {
		$row = array(
			$room->name,
			$room->location,
			$room->indoor ? 'Yes' : 'No',
			$room->capacity,
			'$' . $room->cost . '/hr',
			'<b>Projectors:</b> ' . $room->projector . '<br />' .
			'<b>Whiteboards:</b> ' . $room->whiteboard . '<br />' .
			'<b>Power Plugs:</b> ' . $room->power_source
		);
		if ($with_action) {
			$action = l('View / Book', FACILITIES_MODULE_NAME . '/view/room/' . $room->id);
			$action .= '<br />';
			$action .= l('Edit information', FACILITIES_MODULE_NAME . '/edit/' . $room->id);
			$row[] = $action;
		}
		$rows[] = $row;
		$booking = facilities_render_booking_by_facility_id($room->id, date('Y-m-d'), '2019-1-1');
		$rows[] = array(array('data' => $booking, 'colspan' => 6));
	}
	return theme('table', array('header' => $header, 'rows' => $rows, 'empty' => t('No results found')));
}

function facilities_update_room($values, $id) {
	$qry = db_merge(ROOM_TABLE)
		->key(array('facility_id' => $id))
		->fields(array(
			'projector' => $values['projector'],
			'whiteboard' => $values['whiteboard'],
			'power_source' => $values['power_source']
		));
	return $qry -> execute();
}
/*
 *  ######  ########   #######  ########  ########  ######  
 * ##    ## ##     ## ##     ## ##     ##    ##    ##    ## 
 * ##       ##     ## ##     ## ##     ##    ##    ##       
 *  ######  ########  ##     ## ########     ##     ######  
 *       ## ##        ##     ## ##   ##      ##          ## 
 * ##    ## ##        ##     ## ##    ##     ##    ##    ## 
 *  ######  ##         #######  ##     ##    ##     ######  
 */

function facilities_list_search_form_sports() {
	$form = facilities_list_search_form_common();

	$form['type'] = array(
		'#type' => 'textfield',
		'#title' => 'Facility type',
		'#size' => 50,
		'#autocomplete_path' => FACILITIES_MODULE_NAME . '/auto_complete_type'
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => 'Search'
	);

	return $form;
}


function facilities_list_sports($search) {
	$qry = facilities_list_common_query($search, SPORTS_TABLE);

	if (isset($search['type']) && trim($search['type'])) {
		$qry->condition('special.type', '%' . trim($search['type']) . '%', 'LIKE');
	}

	$res = $qry->execute();

	return facilities_render_sports($res);
}

function facilities_render_sports($entry, $with_action = true) {
	$header = array('Name', 'Location', 'Type', 'Indoor', 'Capacity', 'Cost');
	if ($with_action) {
		$header[] = 'Actions';
	}
	$rows = array();
	foreach($entry as $sports) {
		$row = array(
			$sports->name,
			$sports->location,
			$sports->type,
			$sports->indoor ? 'Yes' : 'No',
			$sports->capacity,
			'$' . $sports->cost . '/hr'
		);
		if ($with_action) {
			$action = l('View / Book', FACILITIES_MODULE_NAME . '/view/sports/' . $sports->id);
			$action .= '<br />';
			$action .= l('Edit information', FACILITIES_MODULE_NAME . '/edit/' . $sports->id);
			$row[] = $action;
		}
		$rows[] = $row;
		$booking = facilities_render_booking_by_facility_id($sports->id, date('Y-m-d'), '2019-1-1');
		$rows[] = array(array('data' => $booking, 'colspan' => 6));
	}
	return theme('table', array('header' => $header, 'rows' => $rows, 'empty' => t('No results found')));
}

function facilities_update_sports($values, $id) {
	$qry = db_merge(SPORTS_TABLE)
		->key(array('facility_id' => $id))
		->fields(array(
			'type' => $values['type']
		));
	return $qry -> execute();
}
/*
 * ########  ########   #######  
 * ##     ## ##     ## ##     ## 
 * ##     ## ##     ## ##     ## 
 * ########  ########  ##     ## 
 * ##     ## ##     ## ##  ## ## 
 * ##     ## ##     ## ##    ##  
 * ########  ########   ##### ## 
 */
function facilities_list_search_form_bbq() {
	$capacity_opt = array('' => 'Any', '0-5' => '< 5', '5-10' => '5 ~ 10',
						  '10-20' => '10 ~ 20', '20' => '> 20');
	$cost_opt = array('' => 'Any', '0' => 'Free', '10' => '< 10',
					  '20' => '< 20', '50' => '< 50');
	$form = facilities_list_search_form_common($capacity_opt, $cost_opt);

	$form['sheltered'] = array(
		'#type' => 'select',
		'#title' => 'Sheltered',
		'#options' => array('' => 'Any', 'yes' => 'Yes', 'no' => 'No'),
		'#default_value' => ''
	);

	$form['water_source'] = array(
		'#type' => 'select',
		'#title' => 'Has water source',
		'#options' => array('' => 'Any', 'yes' => 'Yes', 'no' => 'No'),
		'#default_value' => ''
	);

	$form['grill'] = array(
		'#type' => 'textfield',
		'#title' => 'Grills Required',
		'#size' => 8,
		'#element_validate' => array('facilities_positive_int_field_validator')
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => 'Search'
	);

	return $form;
}


function facilities_list_bbq($search) {
	$qry = facilities_list_common_query($search, BBQ_TABLE);

	
	if (isset($search['sheltered']) && $search['sheltered']) {
		$value = $search['sheltered'] === 'yes' ? 1 : 0;
		$qry->condition('special.sheltered', $value);
	}

	if (isset($search['water_source']) && $search['water_source']) {
		$value = $search['water_source'] === 'yes' ? 1 : 0;
		$qry->condition('special.water_source', $value);
	}

	if (isset($search['grill']) && $search['grill']) {
		$required = intval($search['grill']);
		$qry->condition('special.grill', $required, '>=');
	}
	$res = $qry->execute();

	return facilities_render_bbq($res);
}

function facilities_render_bbq($entry, $with_action = true) {
	$header = array('Name', 'Location', 'Indoor', 'Capacity', 'Cost', 'Resources');
	if ($with_action) {
		$header[] = 'Actions';
	}
	$rows = array();
	foreach($entry as $bbq) {
		$row = array(
			array('data' => $bbq->name, 'rowspan' => '2'),
			$bbq->location,
			$bbq->indoor ? 'Yes' : 'No',
			$bbq->capacity,
			'$' . $bbq->cost . '/hr',
			'<b>Sheltered:</b> ' . ($bbq->sheltered ? 'Yes' : 'No') . '<br />' .
			'<b>Has water source:</b> ' . ($bbq->water_source ? 'Yes' : 'No') . '<br />' .
			'<b>Grills:</b> ' . $bbq->grill
		);
		if ($with_action) {
			$action = l('View / Book', FACILITIES_MODULE_NAME . '/view/bbq/' . $bbq->id);
			$action .= '<br />';
			$action .= l('Edit information', FACILITIES_MODULE_NAME . '/edit/' . $bbq->id);
			$row[] = $action;
		}
		$rows[] = $row;
		$booking = facilities_render_booking_by_facility_id($bbq->id, date('Y-m-d'), '2019-1-1');
		$rows[] = array(array('data' => $booking, 'colspan' => 6));
	}
	return theme('table', array('header' => $header, 'rows' => $rows, 'empty' => t('No results found')));
}

function facilities_update_bbq($values, $id) {
	$qry = db_merge(BBQ_TABLE)
		->key(array('facility_id' => $id))
		->fields(array(
			'sheltered' => $values['sheltered'],
			'water_source' => $values['water_source'],
			'grill' => $values['grill']
		));
	return $qry -> execute();
}


function facilities_get_booking_by_facility_id($facility, $start_date, $end_date) {
	$qry = db_select(BOOKING_TABLE, 'b')
		->condition('b.date', array($start_date, $end_date), 'BETWEEN')
		->condition('b.facility_id', $facility)
		->fields('b', array(
			'user_id',
			'date',
			'start',
			'end'));
	$res = $qry->execute();
	return $res;
}

function facilities_render_booking_by_facility_id($facility, $start_date, $end_date) {
	$bookings = facilities_get_booking_by_facility_id($facility, $start_date, $end_date);
	$output = '';
	$counter = 0;
	foreach ($bookings as $booking) {
		$user = user_load($booking->user_id);
		$output .= $user->name . ': ' .format_date_string($booking->date) . ' ' . $booking->start . ':00 ~ ' . $booking->end . ':00<br/>';
	}
	if (strlen($output) == 0) {
		return 'No bookings found';
	} else {
		return '<b>Bookings:</b><br/>' . $output;
	}
}

function facilities_render_booking_by_user_and_facility_name($facility_name, $uid, $is_admin = false) {
	$qry = db_select(BOOKING_TABLE, 'b');
	$qry->join(FACILITY_TABLE, 'f', 'f.id = b.facility_id');
	$qry->condition('b.date', date('Y-m-d'), '>=');
	if ($facility_name != -1) {
		$qry->condition('f.name', $facility_name);
	}
	if (!$is_admin) {
		$qry->condition('b.user_id', $uid);
	}
	$qry->fields('b')
		->fields('f', array('name'))
		->orderBy('b.date', 'ASC')
		->orderBy('b.start', 'ASC');
	$res = $qry->execute();

	if ($is_admin) {
		$header = array('User', 'Facility', 'Booking Period', 'Actions');
	} else {
		$header = array('Facility', 'Booking Period', 'Actions');
	}
	$rows = [];

	foreach($res as $entry) {
		$username = user_load($entry->user_id)->name;
		$facility_link = l($entry->name, FACILITIES_MODULE_NAME.'/view/'.facilities_get_type_by_id($entry->facility_id).'/'.$entry->facility_id);
		$booking_string = format_date_string($entry->date) . ' ' . $entry->start . ':00 ~ ' . $entry->end . ':00';
		$edit_link = l('Edit', 'booking/edit/' . $entry->id);
		$delete_link = l('Delete', 'booking/delete/' . $entry->id);
		if ($is_admin) {
			$rows[] = array(
				$username,
				$facility_link,
				$booking_string,
				$edit_link . ' - ' . $delete_link
			);
		} else {
			$rows[] = array(
				$facility_link,
				$booking_string,
				$edit_link . ' - ' . $delete_link
			);
		}
	}
	return theme('table', array('header' => $header, 'rows' => $rows, 'empty' => 'No entries found'));
}
function facilities_get_booking_by_id($id) {
	$qry = db_select(BOOKING_TABLE, 'b');
	$qry->join(FACILITY_TABLE, 'f', 'f.id = b.facility_id');
	$qry->condition('b.date', date('Y-m-d'), '>=')
		->condition('b.id', $id)
		->fields('f', array('name'))->fields('b');
	return $qry->execute();
}
function facilities_make_booking($facility, $uid, $date, $from, $to) {
	$cond1 = db_and()
		->condition('b.start', $from, '<=')
		->condition('b.end', $from, '>');
	$cond2 = db_and()
		->condition('b.start', $from, '>=')
		->condition('b.start', $to, '<=');

	$qry = db_select(BOOKING_TABLE, 'b')
		->condition('b.facility_id', $facility)
		->condition('b.date', $date)
		->condition(db_or()
			->condition($cond1)
			->condition($cond2)
			)
		->fields('b', array(
			'start',
			'end'
		));
	$res = $qry->execute();
	if ($res->rowCount() > 0) {
		drupal_set_message('Your booking overlaps with existing entries', 'error');
		return false;
	}
	$qry = db_select(BOOKING_TABLE, 'b')
		->condition('b.facility_id', $facility)
		->condition('b.date', $date)
		->condition('b.user_id', $uid)
		->fields('b', array(
			'id'));
	$res = $qry->execute();
	if ($res->rowCount() > 0) {
		drupal_set_message('You have made an booking for that day', 'error');
		return false;
	}
	$qry = db_insert(BOOKING_TABLE)
		->fields(array(
			'user_id' => $uid,
			'facility_id' => $facility,
			'date' => $date,
			'start' => $from,
			'end' => $to,
		));
	$qry->execute();

	drupal_set_message("Booking successful!");
}


function facilities_modify_booking($booking_id, $facility, $uid, $date, $from, $to) {
	$cond1 = db_and()
		->condition('b.start', $from, '<=')
		->condition('b.end', $from, '>');
	$cond2 = db_and()
		->condition('b.start', $from, '>=')
		->condition('b.start', $to, '<=');

	$qry = db_select(BOOKING_TABLE, 'b')
		->condition('b.facility_id', $facility)
		->condition('b.date', $date)
		->condition(db_or()
			->condition($cond1)
			->condition($cond2)
			)
		->fields('b', array(
			'start',
			'end'
		));
	$res = $qry->execute();
	if ($res->rowCount() > 0) {
		drupal_set_message('Your booking overlaps with existing entries', 'error');
		return false;
	}
	$qry = db_update(BOOKING_TABLE)
		->fields(array(
			'date' => $date,
			'start' => $from,
			'end' => $to,
		))
		->condition('id', $booking_id);
	$qry->execute();
	drupal_set_message("Booking information updated!");
}

/* 
 * ##     ## ######## ##       ########  ######## ########  
 * ##     ## ##       ##       ##     ## ##       ##     ## 
 * ##     ## ##       ##       ##     ## ##       ##     ## 
 * ######### ######   ##       ########  ######   ########  
 * ##     ## ##       ##       ##        ##       ##   ##   
 * ##     ## ##       ##       ##        ##       ##    ##  
 * ##     ## ######## ######## ##        ######## ##     ## 
*/

function facilities_positive_int_field_validator($element, &$form_state, $form) {
	if (empty($element['#value'])) {
		return true;
	}

	if (intval($element['#value']) <= 0 && $element['#value'] != '0') {
		form_error($element, 'Please enter a valid positive integer');
	}
}


function facilities_positive_float_field_validator($element, &$form_state, $form) {
	if (empty($element['#value'])) {
		return true;
	}

	if (floatval($element['#value']) <= 0 && substr($element['#value'], 0, 1) != '0') {
		form_error($element, 'Please enter a valid positive number');
	}
}

function auto_complete_university($string ='') {
	$qry = db_select(FACILITY_TABLE, 'f')
		->fields('f', array(
			'university'
		));
	if (strlen($string) > 1) {
		$qry->condition('f.university', '%'.$string.'%', 'LIKE');
	}
	$qry->distinct();
	$res = $qry -> execute();
	$matches = array();
	foreach($res as $entry) {
		$matches[$entry->university] = check_plain($entry->university);
	}

	drupal_json_output($matches);
}

function auto_complete_location($string ='') {
	$qry = db_select(FACILITY_TABLE, 'f')
		->fields('f', array(
			'location'
		));
	if (strlen($string) > 1) {
		$qry->condition('f.location', '%'.$string.'%', 'LIKE');
	}
	$qry->distinct();
	$res = $qry -> execute();
	$matches = array();
	foreach($res as $entry) {
		$matches[$entry->location] = check_plain($entry->location);
	}

	drupal_json_output($matches);
}

function auto_complete_type($string ='') {
	$qry = db_select(SPORTS_TABLE, 'f')
		->fields('f', array(
			'type'
		));
	if (strlen($string) > 1) {
		$qry->condition('f.type', '%'.$string.'%', 'LIKE');
	}
	$qry->distinct();
	$res = $qry -> execute();
	$matches = array();
	foreach($res as $entry) {
		$matches[$entry->type] = check_plain($entry->type);
	}

	drupal_json_output($matches);
}


function auto_complete_name($string ='') {
	$qry = db_select(FACILITY_TABLE, 'f')
		->fields('f', array(
			'name'
		));
	if (strlen($string) > 1) {
		$qry->condition('f.name', '%'.$string.'%', 'LIKE');
	}
	$qry->distinct();
	$res = $qry -> execute();
	$matches = array();
	foreach($res as $entry) {
		$matches[$entry->name] = check_plain($entry->name);
	}

	drupal_json_output($matches);
}


function format_date_string($str) {
	return date('j M', strtotime($str));
}