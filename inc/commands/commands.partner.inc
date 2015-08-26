<?php

/** list of commands */

function partner_commands() {

  $commands = array(
  	array(
  	  'label' => t('Unpublish all book by partner The American Society of Papyrologists'),
  		'callback' => array(
  		  'unpublish_all_book_by_partner_asp',
  		),
    ),
  );

  return $commands;

}

/**
 * Unpublish by partner
 */
function unpublish_by_partner () {

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
 * Unpublish all book by partner The American Society of Papyrologists
 */
function unpublish_all_book_by_partner_asp () {

	$identifier = 'df20245b-05e6-47b1-ac4c-03811fdd6839' ;

	$books = published_books_by_partner_identifier ( $identifier ) ;

	if ( $books ) {

		foreach ( $books as $book ) {

			// Load the node by NID
			$book_entity = node_load ( $book ) ;

			drush_log ( $book_entity->title , 'ok' ) ;

			// Wrap it with Entity API
			$book_ewrapper = entity_metadata_wrapper('node', $book_entity) ;

			$book_ewrapper->status->set(0) ;

			$book_ewrapper->save() ;

		}

	}

}
