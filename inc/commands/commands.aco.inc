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

  );  

  return $commands;

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
