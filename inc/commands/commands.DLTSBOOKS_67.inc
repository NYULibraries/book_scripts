<?php

/** 
 * https://jira.nyu.edu/browse/DLTSBOOKS-67
 * REPLACE the rights statement that's currently in place for all books in the Indian 
 * Ocean collection on the dev Books site.
 */
function DLTSBOOKS_67_commands() {

  $commands = array(

    array(
      'label' => t('Replace the rights statement according to JIRA ticket DLTSBOOKS-67.'),
      'callback' => array(
        'DLTSBOOKS_67_replace_rights_statement',
      ),
    ),
    
  );  

  return $commands;

}

/** 
 * Indian Ocean
 */
function DLTSBOOKS_67_replace_rights_statement() {

  $collection_identifier = 'd924e139-2636-4e30-95fd-4115c2e41b96';
  
  $collection_nid = collection_nid($collection_identifier);
  
  $replacement_text = "NYU believes the materials displayed on this site are all in the public domain. However, if you believe that you are the copyright owner of any material displayed here, please see our takedown policy: http://dlib.nyu.edu/indianocean/takedownpolicy/";
    
  if ( $collection_nid ) {
    
    drush_print ( 'Books from: ' . collection_title_by_identifier($collection_identifier) );

    $books = collection_books_published($collection_identifier);
    
    foreach ( $books as $book ) {
      
      // Load the node by NID
      $book_entity = node_load($book);
      
      drush_print($book_entity->title);
      
      // Wrap it with Entity API
      $book_ewrapper = entity_metadata_wrapper('node', $book_entity);
      
      $current_text = $book_ewrapper->field_rights->value();
      
      if ( $current_text !== $replacement_text) {

        $book_ewrapper->field_rights->set($replacement_text);
      
        drush_log ( "Rights statement was set to: " . $book_ewrapper->field_rights->value(), 'status' );
        
        $book_ewrapper->save();

      } 
      
      // looks like we already ran this task
      else {
        drush_log ( "Current rights statement is the same as replacement statement.", 'warning' );
      }
    }

  }
  
}
