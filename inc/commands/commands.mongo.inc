<?php

/** list of commands */

function mongo_commands() {

  $commands = array(

    array(
      'label' => t('Export book pages as JSON by book identifier'),
      'callback' => array(
        'mongo_export_book_pages_from_book_by_book_identifier',
      ),
      'arguments' => array ( /** method arguments */
        'identifier' => TRUE , /** true|false to set as require argument */
        'path'  => TRUE , /** true|false to set as require argument */
      ),
    ),

  );  

  return $commands;

}

/** 
 * Export book pages as JSON by book identifier
 * params identifier , path
 */
function mongo_export_book_pages_from_book_by_book_identifier ( $arguments ) {

  $book_pages = array () ;

  $identifier = $arguments['identifier'] ;
  
  $path = $arguments['path'] ;

  $book_nid =  book_nid ( $identifier ) ;
  
  if ( ! is_writable ( $path ) ) {
    drush_log ( dt ( 'Unable to perfom task; cannot write to path "@path" ' , array ( '@path' => $path ) ), 'error') ;
    return ;
  }
  
  if ( ! $book_nid ) {
    drush_log ( dt ( 'Unable to perfom task; cannot find book by identifier "@identifier" ' , array ( '@identifier' => $identifier ) ), 'error') ;
    return ;
  }

  foreach ( book_pages_by_book_nid ( $book_nid ) as $entity ) {
    
    // Wrap it with Entity API
    $entity_ewrapper = entity_metadata_wrapper( 'node', $entity ) ;
    
    $sequence = $entity_ewrapper->field_sequence_number->value() ;
    
    $realPageNumber = $entity_ewrapper->field_real_page_number->value() ;
    
    $croppedMaster = $entity->field_cropped_master[$entity->language][0] ;    
    
    $serviceCopy = $entity_ewrapper->field_service_copy->value() ;    
    
    $serviceCopyURI = str_replace ( 'public:' , 'fileserver:' , $serviceCopy['uri'] ) ;
    
    $croppedMasterURI = str_replace ( 'public:' , 'fileserver:' , $croppedMaster['uri'] ) ;    
    
    $book_pages[] = array (
  	  'isPartOf' => $identifier ,
  	  'sequence' => $sequence ,
  	  'realPageNumber' => $realPageNumber ,
  	  'cm' => array (
  	    'uri' => $croppedMasterURI ,
  		'timestamp' => $croppedMaster['timestamp'] , 
  		'width' => $croppedMaster['djakota_width'] ,
  		'height' => $croppedMaster['djakota_height'] ,
  		'levels' => $croppedMaster['djakota_levels'] ,
  		'dwtLevels' => $croppedMaster['djakota_dwtLevels'] ,
  		'compositingLayerCount' => $croppedMaster['djakota_compositingLayerCount'] ,
  	  ),
  	  'ri' => array ( 'uri' => $serviceCopyURI ) ,
  	  'ocr' => array ( 'uri' => NULL , ) ,
    ) ;
  }
  
  $data = json_encode ( $book_pages ) ;
  
  $done = file_put_contents ( $path . '/' . $identifier . '.json' , $data ) ;
  
  if ( $done ) drush_log ( dt ( 'Done exporting book pages from book @book' , array ( '@book' => $identifier ) ), 'ok') ;
  
  else drush_log ( dt ( 'Unknown error occurred while trying to export book pages from book @book' , array ( '@book' => $identifier ) ), 'error') ;
  
}
