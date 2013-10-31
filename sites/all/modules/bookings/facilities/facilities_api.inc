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

	return $qry;
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
		'#title' => 'Projectors required',
		'#size' => 8,
		'#element_validate' => array('facilities_positive_int_field_validator')
	);

	$form['whiteboard'] = array(
		'#type' => 'textfield',
		'#title' => 'Whiteboards required',
		'#size' => 8,
		'#element_validate' => array('facilities_positive_int_field_validator')
	);

	$form['power_source'] = array(
		'#type' => 'textfield',
		'#title' => 'Power Plugs required',
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

	$header = array('Name', 'Location', 'Indoor', 'Capacity', 'Cost', 'Resources', 'Actions');
	$rows = array();
	foreach($res as $room) {
		$rows[] = array(
			$room->name,
			$room->location,
			$room->indoor ? 'Yes' : 'No',
			$room->capacity,
			'$' . $room->cost . '/hr',
			'<b>Projectors:</b> ' . $room->projector . '<br />' .
			'<b>Whiteboards:</b> ' . $room->whiteboard . '<br />' .
			'<b>Power Plugs:</b> ' . $room->power_source,
			'View / Book'
		);
	}
	return theme('table', array('header' => $header, 'rows' => $rows, 'empty' => t('No results found')));
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
		'#size' => 80
	);

	return $form;
}


function facilities_list_sports($search) {
	$qry = facilities_list_common_query($search, SPORTS_TABLE);

	if (isset($search['type']) && $search['type']) {
		$required = intval($search['type']);
		$qry->condition('special.type', $required);
	}

	$res = $qry->execute();

	$header = array('Name', 'Location', 'Type', 'Indoor', 'Capacity', 'Cost', 'Actions');
	$rows = array();
	foreach($res as $sports) {
		$rows[] = array(
			$sports->name,
			$sports->location,
			$sports->type,
			$sports->indoor ? 'Yes' : 'No',
			$sports->capacity,
			'$' . $sports->cost . '/hr',
			'View / Book'
		);
	}
	return theme('table', array('header' => $header, 'rows' => $rows, 'empty' => t('No results found')));
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
		'#title' => 'Grills required',
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

	$header = array('Name', 'Location', 'Indoor', 'Capacity', 'Cost', 'Resources', 'Actions');
	$rows = array();
	foreach($res as $bbq) {
		$rows[] = array(
			$bbq->name,
			$bbq->location,
			$bbq->indoor ? 'Yes' : 'No',
			$bbq->capacity,
			'$' . $bbq->cost . '/hr',
			'<b>Sheltered:</b> ' . ($bbq->sheltered ? 'Yes' : 'No') . '<br />' .
			'<b>Has water source:</b> ' . ($bbq->water_source ? 'Yes' : 'No') . '<br />' .
			'<b>Grills:</b> ' . $bbq->grill,
			'View / Book'
		);
	}
	return theme('table', array('header' => $header, 'rows' => $rows, 'empty' => t('No results found')));
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