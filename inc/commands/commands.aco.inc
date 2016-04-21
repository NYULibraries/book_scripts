<?php

/** list of commands */

function aco_commands() {

  $commands = array(

    array(
      'label' => t('Update ACO rights field acording to JIRA ticket DLTSACO-111'),
      'callback' => array(
        'update_rights_field_JIRA_DLTSACO_111',
      ),
    ),
    
    array(
      'label' => t('JIRA DLTSACO-319: Number field should be blank for ACO books'),
      'callback' => array(
        'DLTSACO_319',
      ),
    ),
    

  );  

  return $commands;

}

/** 
 * JIRA DLTSACO-319: Number field should be blank for ACO books
 * https://jira.nyu.edu/browse/DLTSACO-319
 *
 * I was looking at the descriptive metadata for nyu_aco000857 in the Viewer and 
 * I saw that the Number field had stuff in it. This field should be blank for all
 * ACO titles. It should be filled in only for The Liberator and any other magazine
 * that comes along in the future.
 */
function DLTSACO_319() {
  $count = 0;
  $collections_identifier = array(
    'f88f5562-2152-41a7-979f-86a6fb4149aa',
    '2b996e54-da86-4622-81d1-090b52e25445',
    '7b71e702-e6b8-4f09-90c9-e5c2906f3050',
    '9c8e89ce-c728-4e1d-9389-48a0222be0a5',
  );
  foreach ($collections_identifier as $identifier) {
    $nid = collection_nid($identifier);
    if ($nid) {
      drush_log(dt('Books from @identifier node/@nid', array('@identifier' => $identifier, '@nid' => $nid)), 'ok');
      foreach (collection_books($identifier) as $book) {
        // Load the node by NID
        $entity = node_load($book);   
        // Wrap it with Entity API
        $wrapper = entity_metadata_wrapper('node', $entity);             
        drush_log(dt('Updating  @title', array( '@title' => $wrapper->title->value(array('sanitize' => TRUE)))), 'ok');
        $wrapper->field_number->set('');
        if ($wrapper->save()) {
          $count++;
        }
        unset($entity);
        unset($wrapper);
        unset($book);
      }
      unset($nid);
    }
  }
  drush_log(dt('@count books where updated', array('@count' => $count)), 'ok');
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
