<?php

/** list of commands */

function commands () {
  
  return array(

    array(
      'label' => 'List collections identifier',
      'callback' => array(
        'list_collections',
      ),
    ),
    
    array(
      'label' => 'List books by collections identifier',
      'callback' => array(
        'list_books_by_collection_identifier',
      ),
    ),    

  );
}

/** commands callback */

function list_collections() {

  $output = '';
  
  $i = 1;
  
  $collections_identifiers = collections_identifiers();

  foreach ( $collections_identifiers as $collections_identifier )  {
  
    // Load the node by NID
    $collection_entity = node_load($collections_identifier);

    // Wrap it with Entity API
    $collection_ewrapper = entity_metadata_wrapper('node', $collection_entity);
    
    $title = $collection_ewrapper->title->value();

    $identifier = $collection_ewrapper->field_identifier->value();
    
    drush_print( $i . '. ' . $title . ' : ' . $identifier);
    
    $i++;
    
  }

}

function list_books_by_collection_identifier( ) {
  
  $traces = debug_backtrace();
  
  $args = array();
  
  foreach ( $traces as $index => $trace ) {
  
    if ( isset( $traces[$index]['function'] ) ) {
      if ( 
        $traces[$index]['function'] == 'drush_core_php_script' 
      ) {
        
        $args = $traces[$index]['args'];

        if ( ($key = array_search('script.php', $args)) !== false) {
          unset($args[$key]);
        }        
        
      }
    }
  }
  
  foreach ( $args as $arg ) {
  
    $collection_nid = collection_nid( $arg );
    
    if ( $collection_nid ) {
    
      drush_print ( 'Books from: ' . collection_title_by_identifier($arg) );

      $books = collection_books($arg);
      
      foreach ( $books as $book ) {
      
        // Load the node by NID
        $book_entity = node_load($book);

        // Wrap it with Entity API
        $book_ewrapper = entity_metadata_wrapper('node', $book_entity);
        
        drush_print( $book_ewrapper->title->value(array('sanitize' => TRUE)) );
        
        // $book_ewrapper->status->set(0);
        
        // $book_ewrapper->save();

      }

    }

  }
  
}