<?php

/** list of commands */

function DLTSACO_114_commands() {

  $commands = array(

    array(
      'label' => t('Generate list of book IDs of all published ACO books (DLTSACO-114)'),
      'callback' => array(
        'DLTSACO_114_list_books',
      ),
    ),

  );  

  return $commands;

}

/** 
 * Update Rights field for ACO books
 * https://jira.nyu.edu/browse/DLTSACO-111
 */
function DLTSACO_114_list_books() {

  $collection_identifier = '7b71e702-e6b8-4f09-90c9-e5c2906f3050';
  
  $collection_nid = collection_nid($collection_identifier);
    
  if ( $collection_nid ) {
    
    drush_print ( 'Books from: ' . collection_title_by_identifier($collection_identifier) );

    $books = collection_books_published($collection_identifier);
      
    foreach ( $books as $book ) {
      
      // Load the node by NID
      $book_entity = node_load($book);

      // Wrap it with Entity API
      $book_ewrapper = entity_metadata_wrapper('node', $book_entity);
      
      $book_identifier = $book_ewrapper->field_identifier->value();
        
      drush_print( $book_identifier );
      
    }

  }
  
}
