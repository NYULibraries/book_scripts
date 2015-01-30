<?php

/** list of commands */

function DLTSACO_XXX_commands() {

  $commands = array(

    array(
      'label' => t('Unpublish all books from Indian Ocean books'),
      'callback' => array(
        'DLTSACO_XXX_list_books',
      ),
    ),
    
    array(
      'label' => t('Unpublish all books from The Newsletter of the Circle of Inner Asian Art'),
      'callback' => array(
        'DLTSACO_ZZZ_list_books',
      ),
    ),
    
    array(
      'label' => t('Unpublish all books from Ancient World Digital Library'),
      'callback' => array(
        'DLTSACO_AAA_list_books',
      ),
    ),    

    array(
      'label' => t('Unpublish all books from Alfred C. Berol Collection of Lewis Carroll'),
      'callback' => array(
        'DLTSACO_BBB_list_books',
      ),
    ),        
    
    array(
      'label' => t('Unpublish all books from The Liberator'),
      'callback' => array(
        'DLTSACO_CCC_list_books',
      ),
    ),        

  );  

  return $commands;

}

/** 
 * Indian Ocean
 */
function DLTSACO_XXX_list_books() {

  $collection_identifier = 'd924e139-2636-4e30-95fd-4115c2e41b96';
  
  $collection_nid = collection_nid($collection_identifier);
    
  if ( $collection_nid ) {
    
    drush_print ( 'Books from: ' . collection_title_by_identifier($collection_identifier) );

    $books = collection_books_published($collection_identifier);
    
    foreach ( $books as $book ) {
      
      // Load the node by NID
      $book_entity = node_load($book);
      
      drush_print($book_entity->title);
      
      // Wrap it with Entity API
      $book_ewrapper = entity_metadata_wrapper('node', $book_entity);
      
      $book_ewrapper->status->set(0);
      
      $book_ewrapper->save();
      
    }

  }
  
}

/** 
 * The Newsletter of the Circle of Inner Asian Art
 */
function DLTSACO_YYY_list_books() {

  $collection_identifier = '9e19af0a-3951-4cd4-9fb6-958d9187acce';
  
  $collection_nid = collection_nid($collection_identifier);
    
  if ( $collection_nid ) {
    
    drush_print ( 'Books from: ' . collection_title_by_identifier($collection_identifier) );

    $books = collection_books_published($collection_identifier);
    
    foreach ( $books as $book ) {
      
      // Load the node by NID
      $book_entity = node_load($book);
      
      drush_print($book_entity->title);
      
      // Wrap it with Entity API
      $book_ewrapper = entity_metadata_wrapper('node', $book_entity);
      
      $book_ewrapper->status->set(0);
      
      $book_ewrapper->save();
      
    }

  }
  
}

/** 
 * Ancient World Digital Library
 */
function DLTSACO_AAA_list_books() {

  $collection_identifier = 'd72398c3-1cd7-4734-ab1c-3e3b29862cee';

  $collection_nid = collection_nid($collection_identifier);
    
  if ( $collection_nid ) {
    
    drush_print ( 'Books from: ' . collection_title_by_identifier($collection_identifier) );

    $books = collection_books_published($collection_identifier);
    
    foreach ( $books as $book ) {
      
      // Load the node by NID
      $book_entity = node_load($book);
      
      drush_print($book_entity->title);
      
      // Wrap it with Entity API
      $book_ewrapper = entity_metadata_wrapper('node', $book_entity);
      
      $book_ewrapper->status->set(0);
      
      $book_ewrapper->save();
      
    }

  }
  
}

/** 
 * Alfred C. Berol Collection of Lewis Carroll
 */
function DLTSACO_BBB_list_books() {

  $collection_identifier = 'ea27bbb6-c0e6-4a07-a87c-f9e3aa990b47';

  $collection_nid = collection_nid($collection_identifier);
    
  if ( $collection_nid ) {
    
    drush_print ( 'Books from: ' . collection_title_by_identifier($collection_identifier) );

    $books = collection_books_published($collection_identifier);
    
    foreach ( $books as $book ) {
      
      // Load the node by NID
      $book_entity = node_load($book);
      
      drush_print($book_entity->title);
      
      // Wrap it with Entity API
      $book_ewrapper = entity_metadata_wrapper('node', $book_entity);
      
      $book_ewrapper->status->set(0);
      
      $book_ewrapper->save();
      
    }

  }
  
}

/** 
 * The Liberator
 */
function DLTSACO_CCC_list_books() {

  $collection_identifier = '94b17163-75c2-42c4-b258-754e9fc9e0ea';

  $collection_nid = collection_nid($collection_identifier);
    
  if ( $collection_nid ) {
    
    drush_print ( 'Books from: ' . collection_title_by_identifier($collection_identifier) );

    $books = collection_books_published($collection_identifier);
    
    foreach ( $books as $book ) {
      
      // Load the node by NID
      $book_entity = node_load($book);
      
      drush_print($book_entity->title);
      
      // Wrap it with Entity API
      $book_ewrapper = entity_metadata_wrapper('node', $book_entity);
      
      $book_ewrapper->status->set(0);
      
      $book_ewrapper->save();
      
    }

  }
  
}
