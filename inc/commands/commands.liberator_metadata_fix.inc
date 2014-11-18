<?php

/** list of commands */

function liberator_metadata_fix_commands() {

  return array(

    array(
      'label' => t('Update Liberator books metadata according to DLTSBOOKS-39'),
      'callback' => array(
        'liberator_metadata_fix_fix_books',
      ),
    ),
    
    array(
      'label' => t('Update Liberator Multi Vol. Books title according to DLTSBOOKS-39'),
      'callback' => array(
        'liberator_metadata_fix_fix_multivol_books',
      ),
    ),

  );

}

/** commands callback */

function liberator_metadata_fix_fix_books() {

  $books = collection_books('94b17163-75c2-42c4-b258-754e9fc9e0ea');
  
  foreach ( $books as $book ) {
    
    // Load the node by NID
    $entity = node_load($book);
    
    // Wrap it with Entity API
    $ewrapper = entity_metadata_wrapper('node', $entity);
    
    $title = $ewrapper->title->value();
    
    $title_long = $ewrapper->field_title->value();
    
    $ewrapper->title->set( $title );
    
    $ewrapper->field_title->set( $title_long );

    foreach ($ewrapper->field_publisher->getIterator() as $delta => $field_wrapper) {
      
      $publisher = str_replace(']', '', $field_wrapper->value());
      
      $field_wrapper->set($publisher);
      
    }    
    
    $ewrapper->save(); 
    
  }     

}

function liberator_metadata_fix_fix_multivol_books() {

  $books = collection_books('94b17163-75c2-42c4-b258-754e9fc9e0ea');
  
  foreach ( $books as $book ) {
  
    $multivol_book_nid = multivol_book_by_book_nid($book);
    
    // Load the node by NID
    $entity = node_load($multivol_book_nid);
    
    // Wrap it with Entity API
    $ewrapper = entity_metadata_wrapper('node', $entity);
    
    $title = $ewrapper->title->value();
    
    $ewrapper->title->set( 'The Liberator' );
    
    $ewrapper->save();     
    
  }     

}
