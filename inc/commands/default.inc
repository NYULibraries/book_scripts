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
    
    array(
      'label' => t('Given a book identifier generate all derivative image'),
      'callback' => array(
        'generate_book_pages_representative_image_thumbnails',
      ),
    ),
    
    array(
      'label' => t('Unpublish all maps in a collection'),
      'callback' => array(
        'unpublish_maps',
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

function unpublish_maps ( ) {
  
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

      $maps = collection_maps($arg);      
      
      foreach ( $maps as $map ) {
      
        // Load the node by NID
        $map_entity = node_load($map);

        // Wrap it with Entity API
        $map_ewrapper = entity_metadata_wrapper('node', $map_entity);
        
        $map_ewrapper->status->set(0);
        
        $map_ewrapper->save(); 

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
 * Given a book identifier, generate all derivative image.
 */
function generate_book_pages_representative_image_thumbnails ( ) {

  if ( ! $book_identifier = drush_get_option('book_identifier') ) $book_identifier = drush_prompt( 'Book identifier', NULL, TRUE ) ;
  
  if ( ! $book_identifier ) return drush_log ( 'Invalid book identifier' ) ;

  $book_pages = drush_get_option('book_pages') ;  
  
  $book_nid = book_nid( $book_identifier );
  
  if ( $book_nid ) {
  
    if ( empty ( $book_pages ) ) $book_pages = book_pages_by_book_nid ( $book_nid ) ;
    
    else $book_pages = book_pages_by_book_nid_and_sequence_numbers ( $book_nid, explode ( ',', $book_pages ) ) ;
    
    // Load the node by NID
    $book_entity = node_load( $book_nid );

    // Wrap it with Entity API
    $book_ewrapper = entity_metadata_wrapper('node', $book_entity);
        
    drush_log( 'Book: ' . $book_ewrapper->title->value(array('sanitize' => TRUE)), 'status' );
    
    $styles = array_keys ( image_styles() ) ;
    
    $styles_string = implode(', ', $styles ) ;
    
    foreach ( $book_pages as $page ) {
    
      // Wrap it with Entity API
      $book_page_ewrapper = entity_metadata_wrapper('node', $page);
    
      $sequence_number = $book_page_ewrapper->field_sequence_number->value();
      
      $service_copy = $book_page_ewrapper->field_service_copy->value();
      
      drush_log( dt('Book page @page: Generating image styles: @styles', array( '@page' => $sequence_number, '@styles' => $styles_string ) ) , 'status' );    
      
      foreach ( $styles as $style ) {

       // destination of the derivative image.
       $destination = image_style_path($style, $service_copy['uri']);
       
       // remove file if exists
       if ( file_exists ( $destination ) ) file_unmanaged_delete ( $destination ) ;
       
       // create derivative image.
       image_style_create_derivative( image_style_load( $style ), $service_copy['uri'], $destination);
       
       // save node
       $book_page_ewrapper->save();
       
      }
        
    }
        
  }
  
  else return drush_log ( dt('Invalid book @identifier' , array ( '@identifier' => $book_identifier ) ), 'error' ) ;
  
}
