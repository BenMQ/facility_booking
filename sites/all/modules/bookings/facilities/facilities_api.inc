<?php
require_once(drupal_get_path('module', 'facilities') . '/facilities_constants.php');

function facilities_list_rooms() {
	$qry = db_select(FACILITY_TABLE, 'f');
	$qry->join(ROOM_TABLE, 'r', 'r.facility_id = f.id');
	$qry->fields('f')
		->fields('r');
	$res = $qry->execute();

	$header = array('Name', 'Location', 'Indoor', 'Capacity', 'Cost', 'Resources', 'Upcoming Bookings');
	$rows = array();
	foreach($res as $room) {
		$rows[] = array(
			$room->name,
			$room->location,
			$room->indoor ? 'Yes' : 'No',
			$room->capacity,
			'$' . $room->cost,
			'<b>Projectors:</b> ' . $room->projector . '<br />' .
			'<b>Whiteboards:</b> ' . $room->whiteboard . '<br />' .
			'<b>Power Plugs:</b> ' . $room->power_source,
			'none'
		);
	}
	return theme('table', array('header' => $header, 'rows' => $rows, 'empty' => t('No results found')));
}
