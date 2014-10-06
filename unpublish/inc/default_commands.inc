<?php

/** list of commands */

function default_commands () {
  return array(

    array(
      'label' => t('List collections identifiers'),
      'callback' => array(
        'list_collections',
      ),
    ),

    array(
      'label' => t('Unpublish books by collection identifier'),
      'callback' => array(
        'unpublish_books',
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

function unpublish_books ( ) {
  
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

      $books = collection_books($arg);
      
      foreach ( $books as $book ) {
      
        // Load the node by NID
        $book_entity = node_load($book);

        // Wrap it with Entity API
        $book_ewrapper = entity_metadata_wrapper('node', $book_entity);
        
        $book_ewrapper->status->set(0);
        
        $book_ewrapper->save(); 

      }

    }

  }
  
}