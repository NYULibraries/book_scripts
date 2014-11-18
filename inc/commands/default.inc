<?php

/** list of commands */

function default_commands() {

  $commands = array(

    array(
      'label' => t('List collections identifiers'),
      'callback' => array(
        'list_collections',
      ),
    ),

    array(
      'label' => t('Mark all books in a collection as published'),
      'callback' => array(
        'publish_books',
      ),
    ),

    array(
      'label' => t('Unpublish all books in a collection'),
      'callback' => array(
        'unpublish_books',
      ),
    ),
    
  );  

  foreach ( file_scan_directory( __DIR__, '/commands.(.*)\.inc$/', array('recurse' => FALSE )) as $file) {

    include_once($file->uri);

    $function_to_call = str_replace('commands.', '', $file->name . '_commands');

    if ( function_exists($function_to_call) ) {

      $this_commands = $function_to_call();
      
      foreach ( $this_commands as $this_command ) {
        $commands[] = $this_command;
      }
      
    }

  }
  
  return $commands;

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

function publish_books ( ) {
  
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
        
        drush_print($book_entity->title);

        // Wrap it with Entity API
        $book_ewrapper = entity_metadata_wrapper('node', $book_entity);
        
        $book_ewrapper->status->set(1);
        
        $book_ewrapper->save(); 

      }

    }

  }
  
}
