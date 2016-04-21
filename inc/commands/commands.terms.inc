<?php

/** list of commands */

function terms_commands() {
  $commands = array(
    array(
      'label' => t('List all the updated modules'),
      'callback' => array(
        'list_updated_nodes',
      ),
    ),

  );
  return $commands;

}

function list_updated_nodes() {
	$query = db_query('SELECT node.nid AS nid FROM node WHERE node.created <> node.changed AND node.type = :type', array(':type' => 'dlts_book'));
	$data = array();
	foreach ($query->fetchAll() as $record) {
		$data[$record->nid] = array();
		$data[$record->nid]['url'] = url(drupal_get_path_alias('node/'.$record->nid), array('absolute' => true));
		$data[$record->nid]['nid'] = $record->nid;
		$data[$record->nid]['subject'] = array();
		// Load the node by NID
	  $entity = node_load($record->nid);
	  // Wrap it with Entity API
	  $wrapper = entity_metadata_wrapper('node', $entity);
	  $identifier = $wrapper->field_identifier->value();
	  $data[$record->nid]['identifier'] = $identifier;
	  // Subjects
	  $subjects = $wrapper->field_subject->value();
	  foreach ($subjects as $subject) {
	  	$data[$record->nid]['subject'][$subject->tid] = array();
	  	$data[$record->nid]['subject'][$subject->tid]['tid'] = $subject->tid;
	  	$data[$record->nid]['subject'][$subject->tid]['vid'] = $subject->vid;
	  	$data[$record->nid]['subject'][$subject->tid]['name'] = $subject->name;
	  }
	}
	if (file_unmanaged_save_data(json_encode($data), variable_get('file_public_path', conf_path() . '/files') . '/terms_data.json', FILE_EXISTS_REPLACE) ) {
	  drush_log('ok', 'ok');
	}
}
