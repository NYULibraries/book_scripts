<?php

/** list of commands */

function mongo_commands() {

  $commands = array (
    /**
    array (
      'label' => t('Export book pages as JSON by book identifier'),
      'callback' => array(
        'mongo_export_book_pages_from_book_by_book_identifier',
      ),
      'arguments' => array ( // method arguments
        'identifier' => TRUE , // true|false to set as require argument
        'path'  => TRUE , // true|false to set as require argument
      ),
    ),
    */ 
    array (
      'label' => t('Migrate single book pages by book identifier'),
      'callback' => array(
        'mongo_migrate_single_book_pages_from_book_by_book_identifier',
      ),
      'arguments' => array ( /** method arguments */
        'identifier' => TRUE , /** true|false to set as require argument */
      ),
    ),
    
    array (
      'label' => t('Migrate stitched book pages by book identifier'),
      'callback' => array (
        'mongo_migrate_stitched_book_pages_from_book_by_book_identifier',
      ),
      'arguments' => array ( /** method arguments */
        'identifier' => TRUE , /** true|false to set as require argument */
      ),
    ),    

    array (
      'label' => t('Delete jp2 images'),
      'callback' => array (
        'mongo_delete_jp2_images',
      ),
      'arguments' => array (),
    ),    
        
  );  

  return $commands ;

}

function mongo_delete_jp2_images ( ) {

  /** Remove anything left behind */
  $query = "DELETE FROM field_data_field_cropped_master WHERE bundle = 'dlts_book_page' ;" ;

  db_query ( $query ) ;
  
  $query = "DELETE FROM field_revision_field_cropped_master WHERE bundle = 'dlts_book_page' ;" ;

  db_query ( $query ) ;

  $query = "DELETE FROM field_data_field_stitch_image WHERE bundle = 'dlts_book_stitched_page' ;" ;

  db_query ( $query ) ;

  $query = "DELETE FROM field_revision_field_stitch_image WHERE bundle = 'dlts_book_stitched_page' ;" ;

  db_query ( $query ) ;

  /** We still have Maps make a list of Maps cropped_master */
  $query = "SELECT DISTINCT field_cropped_master_fid fid FROM field_revision_field_cropped_master WHERE bundle = 'dlts_map' ; " ;
  
  $results = db_query ( $query ) ;
  
  $maps = array ( ) ;
  
  foreach ( $results as $result ) {
    $maps[] = $result->fid ;
  } 
  
  $query = "
    SELECT file_managed.fid 
      FROM file_managed 
      LEFT JOIN file_usage
        ON file_usage.fid = file_managed.fid 
      WHERE file_managed.filemime = 'application/octet-stream' ;
  ";
  
  $results = db_query ( $query ) ;
  
  foreach ( $results as $result ) {
    if ( ! in_array ( $result->fid , $maps ) ) {
    
      $file =  file_load ( $result->fid ) ;

      drush_log ( dt ('Delete file by fid @fid.' , array ( '@fid' => $result->fid ) ) , 'ok' ) ;

      module_invoke_all('file_delete', $file);
      
      module_invoke_all('entity_delete', $file, 'file');      
      
      db_delete('file_managed')->condition('fid', $result->fid )->execute();
      
      db_delete('file_usage')->condition('fid', $result->fid )->execute();
      
    }
  }
  
  entity_get_controller('file')->resetCache() ;

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
    
    $serviceCopyURI = str_replace ( 'public://' , 'fileserver://books/' , $serviceCopy['uri'] ) ;
    
    $croppedMasterURI = str_replace ( 'public://' , 'fileserver://books/' , $croppedMaster['uri'] ) ;    
    
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

/** 
 * Export single book pages as JSON by book identifier
 * params identifier , path
 */
function mongo_migrate_single_book_pages_from_book_by_book_identifier ( $arguments = array () ) {

  $collection_name = 'dlts_book_page' ;

  $book_pages = array () ;

  $identifier = $arguments['identifier'] ;
  
  if ( empty ( $identifier) ) {
    drush_log ( dt ( 'No identifier') ) ;
    return ;
  }
  
  $book_nid =  book_nid ( $identifier ) ;
  
  if ( ! $book_nid ) {
    drush_log ( dt ( 'Unable to perfom task; cannot find book by identifier "@identifier" ' , array ( '@identifier' => $identifier ) ), 'error') ;
    return ;
  }
  
  $book_pages_by_book_nid = book_pages_by_book_nid ( $book_nid ) ;
  
  if ( ! $book_pages_by_book_nid ) return ;

  foreach ( book_pages_by_book_nid ( $book_nid ) as $entity ) {
    
    drush_log ( dt ( 'Migrating book pages from book @book' , array ( '@book' => $entity->title ) ), 'ok') ;
    
    // Wrap it with Entity API
    $entity_ewrapper = entity_metadata_wrapper( 'node', $entity ) ;
    
    $sequence = $entity_ewrapper->field_sequence_number->value() ;
    
    $realPageNumber = $entity_ewrapper->field_real_page_number->value() ;
    
    $croppedMaster = $entity->field_cropped_master[$entity->language][0] ;    
    
    $serviceCopy = $entity_ewrapper->field_service_copy->value() ;    
    
    $serviceCopyURI = str_replace ( 'public://' , 'fileserver://books/' , $serviceCopy['uri'] ) ;
    
    $croppedMasterURI = str_replace ( 'public://' , 'fileserver://books/' , $croppedMaster['uri'] ) ;    
    
    $query = array (
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
    
    mongodb_collection ( $collection_name )->insert( $query ) ;

    node_delete ( $entity->nid ) ;

  }
  
  drush_log ( dt ( 'Done exporting book pages from book @book' , array ( '@book' => $identifier ) ), 'ok') ;
  
}

/** 
 * Export stitched book pages as JSON by book identifier
 * params identifier , path
 * @TODO: We will change the way this works
 */
function mongo_migrate_stitched_book_pages_from_book_by_book_identifier ( $arguments = array () ) {

  $collection_name = 'dlts_book_stitched_page' ;

  $book_pages = array () ;

  $identifier = $arguments['identifier'] ;
  
  if ( empty ( $identifier) ) {
    drush_log ( dt ( 'No identifier') ) ;
    return ;
  }
  
  $book_nid =  book_nid ( $identifier ) ;
  
  if ( ! $book_nid ) {
    drush_log ( dt ( 'Unable to perfom task; cannot find book by identifier "@identifier" ' , array ( '@identifier' => $identifier ) ), 'error') ;
    return ;
  }
  
  $book_stitched_page_by_book_nid = book_stitched_pages_by_book_nid ( $book_nid ) ;
  
  if ( ! $book_stitched_page_by_book_nid ) return ;

  foreach ( $book_stitched_page_by_book_nid as $entity ) {
    
    drush_log ( dt ( 'Migrating book stitched pages from book @book' , array ( '@book' => $entity->title ) ), 'ok') ;
    
    // Wrap it with Entity API
    $entity_ewrapper = entity_metadata_wrapper( 'node', $entity ) ;
    
    $left = $entity_ewrapper->field_sequence_number_left->value() ;
    
    $right = $entity_ewrapper->field_sequence_number_right->value() ;    
    
    $stitchImage = $entity->field_stitch_image[$entity->language][0] ;    
    
    $stitchImageURI = str_replace ( 'public://' , 'fileserver://books/' , $stitchImage['uri'] ) ;
    
    $query = array (
  	  'isPartOf' => $identifier ,
  	  'sequence' => array ( $left , $right ) ,
  	  'type' => 'stitched_page',
  	  'cm' => array (
  	    'uri' => $stitchImageURI ,
  		'timestamp' => $stitchImage['timestamp'] , 
  		'width' => $stitchImage['djakota_width'] ,
  		'height' => $stitchImage['djakota_height'] ,
  		'levels' => $stitchImage['djakota_levels'] ,
  		'dwtLevels' => $stitchImage['djakota_dwtLevels'] ,
  		'compositingLayerCount' => $stitchImage['djakota_compositingLayerCount'] ,
  	  ),
    ) ;
    
    mongodb_collection ( $collection_name )->insert( $query ) ;

    node_delete ( $entity->nid ) ;

  }
  
  drush_log ( dt ( 'Done migrating book stitched pages from book @book' , array ( '@book' => $identifier ) ), 'ok') ;
  
}
