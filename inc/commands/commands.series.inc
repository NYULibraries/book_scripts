<?php

/** list of commands */

function series_commands() {

  $commands = array(

    array(
      'label' => t('List series'),
      'callback' => array(
        'list_series',
      ),
    ),

  );  

  return $commands;

}

/** 
 * List all the series in the site
 */
function list_series () {

  $a = array () ;

  foreach ( get_series_book ( ) as $series_book ) {
      
    // Load the node by NID
    $entity = node_load ( $series_book ) ;
    
    // A series book have a reference to a dlts_book and a dlts_series
    // get it.

    // Wrap it with Entity API
    $entity_ewrapper = entity_metadata_wrapper( 'node', $entity ) ;
    
    $series_entity = $entity_ewrapper->field_series->value();
    
    // Wrap it with Entity API
    $series_ewrapper = entity_metadata_wrapper( 'node', $series_entity ) ;
    
    $identifier = $series_ewrapper->field_identifier->value() ;
    
    if ( ! isset ( $a[$identifier] ) ) $a[$identifier] = 1 ;

    else $a[$identifier] = $a[$identifier] + 1 ;
        
  }
  
  drush_print_r ( $a ) ;
  
}
