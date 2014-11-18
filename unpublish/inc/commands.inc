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
    
    array(
      'label' => t('Update ACO rights field acording to JIRA ticket DLTSACO-111'),
      'callback' => array(
        'update_rights_field_JIRA_DLTSACO_111',
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

/** 
 * Update Rights field for ACO books
 * https://jira.nyu.edu/browse/DLTSACO-111
 */
function update_rights_field_JIRA_DLTSACO_111() {

  $collection_identifier = '7b71e702-e6b8-4f09-90c9-e5c2906f3050';
  
  $collection_nid = collection_nid($collection_identifier);
    
  if ( $collection_nid ) {
    
    drush_print ( 'Books from: ' . collection_title_by_identifier($collection_identifier) );

    $books = collection_books($collection_identifier);
      
    foreach ( $books as $book ) {
      
      // Load the node by NID
      $book_entity = node_load($book);

      // Wrap it with Entity API
      $book_ewrapper = entity_metadata_wrapper('node', $book_entity);
        
      drush_print( 'Updating: ' . $book_ewrapper->title->value(array('sanitize' => TRUE)) );
        
      $plain_text = "NYU believes the materials displayed on this site are all in the public domain. However, if you believe that you are the copyright owner of any material displayed here, please see our takedown policy: http://dlib.nyu.edu/aco/takedownpolicy/";
        
      $current_value = $book_ewrapper->field_rights->value();
        
      $book_ewrapper->field_rights->set($plain_text);
        
      $book_ewrapper->save();

    }

  }
  
}