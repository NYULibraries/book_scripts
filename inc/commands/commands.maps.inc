<?php

/** list of commands */

function maps_commands() {

  $commands = array(
  	array(
  	  'label' => t('Unpublish all maps'),
  		'callback' => array(
  		  'unpublish_all_maps',
  		),
    ),
  	/**
    array(
      'label' => t('Unpublish all maps from Indian Ocean books'),
      'callback' => array(
        'unpublish_all_maps_indianocean',
      ),
    ),
    array(
      'label' => t('Unpublish all maps from The Newsletter of the Circle of Inner Asian Art'),
      'callback' => array(
        'unpublish_all_maps_nciaa',
      ),
    ),
    array(
      'label' => t('Unpublish all maps from Ancient World Digital Library'),
      'callback' => array(
        'unpublish_all_maps_awdl',
      ),
    ),
    array(
      'label' => t('Unpublish all maps from Alfred C. Berol Collection of Lewis Carroll'),
      'callback' => array(
        'unpublish_all_maps_acbclc',
      ),
    ),
    array(
      'label' => t('Unpublish all maps from The Liberator'),
      'callback' => array(
        'unpublish_all_maps_theliberator',
      ),
    ),
    */
  );

  return $commands;

}

/**
 * Unpublish all maps
 */
function unpublish_all_maps () {

	foreach ( collections_identifiers ( ) as $collection_nid ) {

	  $collection_node = node_load ( $collection_nid ) ;

	  $collection_wrapper = entity_metadata_wrapper ( 'node' , $collection_node ) ;

	  $maps = collection_maps_published ( $collection_wrapper->field_identifier->value() ) ;

		if ( $maps ) {

			drush_log ( dt ( '@title' , array ( '@title' => $collection_node->title ) ) , 'ok' ) ;

		  foreach ( $maps as $map ) {
        // Load the node by NID
		    $map_entity = node_load ( $map ) ;

		    drush_log ( $map_entity->title , 'ok' ) ;

		    // Wrap it with Entity API
		    $map_ewrapper = entity_metadata_wrapper('node', $map_entity) ;

		    $map_ewrapper->status->set(0) ;

		    $map_ewrapper->save() ;

		  }

		}

	}
}

/**
 * Indian Ocean
 */
function unpublish_all_maps_indianocean () {

	return ;

  $collection_identifier = 'd924e139-2636-4e30-95fd-4115c2e41b96';

  $collection_nid = collection_nid($collection_identifier);

  if ( $collection_nid ) {

    drush_print ( 'Maps from: ' . collection_title_by_identifier($collection_identifier) );

    $maps = collection_maps_published($collection_identifier);

    foreach ( $maps as $map ) {

      // Load the node by NID
      $map_entity = node_load($map);

      drush_print($map_entity->title);

      // Wrap it with Entity API
      $map_ewrapper = entity_metadata_wrapper('node', $map_entity);

      $map_ewrapper->status->set(0);

      $map_ewrapper->save();

    }

  }

}

/**
 * The Newsletter of the Circle of Inner Asian Art
 */
function unpublish_all_maps_nciaa() {

	return ;

  $collection_identifier = '9e19af0a-3951-4cd4-9fb6-958d9187acce';

  $collection_nid = collection_nid($collection_identifier);

  if ( $collection_nid ) {

    drush_print ( 'Maps from: ' . collection_title_by_identifier($collection_identifier) );

    $maps = collection_maps_published($collection_identifier);

    foreach ( $maps as $map ) {

      // Load the node by NID
      $map_entity = node_load($map);

      drush_print($map_entity->title);

      // Wrap it with Entity API
      $map_ewrapper = entity_metadata_wrapper('node', $map_entity);

      $map_ewrapper->status->set(0);

      $map_ewrapper->save();

    }

  }

}

/**
 * Ancient World Digital Library
 */
function unpublish_all_maps_awdl() {

	return ;

  $collection_identifier = 'd72398c3-1cd7-4734-ab1c-3e3b29862cee';

  $collection_nid = collection_nid($collection_identifier);

  if ( $collection_nid ) {

    drush_print ( 'Maps from: ' . collection_title_by_identifier($collection_identifier) );

    $maps = collection_maps_published($collection_identifier);

    foreach ( $maps as $map ) {

      // Load the node by NID
      $map_entity = node_load($map);

      drush_print($map_entity->title);

      // Wrap it with Entity API
      $map_ewrapper = entity_metadata_wrapper('node', $map_entity);

      $map_ewrapper->status->set(0);

      $map_ewrapper->save();

    }

  }

}

/**
 * Alfred C. Berol Collection of Lewis Carroll
 */
function unpublish_all_maps_acbclc() {

	return ;

  $collection_identifier = 'ea27bbb6-c0e6-4a07-a87c-f9e3aa990b47';

  $collection_nid = collection_nid($collection_identifier);

  if ( $collection_nid ) {

    drush_print ( 'Maps from: ' . collection_title_by_identifier($collection_identifier) );

    $maps = collection_maps_published($collection_identifier);

    foreach ( $maps as $map ) {

      // Load the node by NID
      $map_entity = node_load($map);

      drush_print($map_entity->title);

      // Wrap it with Entity API
      $map_ewrapper = entity_metadata_wrapper('node', $map_entity);

      $map_ewrapper->status->set(0);

      $map_ewrapper->save();

    }

  }

}

/**
 * The Liberator
 */
function unpublish_all_maps_theliberator () {

	return ;

  $collection_identifier = '94b17163-75c2-42c4-b258-754e9fc9e0ea' ;

  $collection_nid = collection_nid($collection_identifier);

  if ( $collection_nid ) {

    drush_print ( 'Maps from: ' . collection_title_by_identifier($collection_identifier) );

    $maps = collection_maps_published($collection_identifier);

    foreach ( $maps as $map ) {

      // Load the node by NID
      $map_entity = node_load($map);

      drush_print($map_entity->title);

      // Wrap it with Entity API
      $map_ewrapper = entity_metadata_wrapper('node', $map_entity);

      $map_ewrapper->status->set(0);

      $map_ewrapper->save();

    }

  }

}
