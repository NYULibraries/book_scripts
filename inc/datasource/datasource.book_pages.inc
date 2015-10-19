<?php

function book_page_nid ( $book_nid, $sequence_number ) {

  $query = new EntityFieldQuery;

  $result = $query
    ->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'dlts_book_page')
    ->fieldCondition('field_sequence_number', 'value', $sequence_number, '=')
    ->fieldCondition('field_book', 'nid', $book_nid, '=')    
    ->execute();
    
  if (empty($result['node'])) return FALSE;

  $keys = array_keys($result['node']);

  return (int)$keys[0];

}

function book_pages_nid_by_book_identifier ( $book_nid ) {

  $query = new EntityFieldQuery;

  $result = $query
    ->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'dlts_book_page')
    ->fieldCondition('field_book', 'nid', $book_nid, '=')    
    ->execute();
    
  if ( empty( $result['node'] ) ) return FALSE;

  return array_keys( $result['node'] );

}

function book_stitched_pages_by_book_nid ( $book_nid ) {

  $pages = array () ;

  $query = new EntityFieldQuery;

  $result = $query
    ->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'dlts_book_stitched_page')
    ->fieldCondition('field_book', 'nid', $book_nid, '=')    
    ->execute();

  if ( empty( $result['node'] ) ) return FALSE;
  
  foreach ( $result['node'] as $node ) $pages[$node->nid] =  node_load( $node->nid ) ;

  return $pages ;

}

function book_pages_by_book_nid_and_sequence_numbers ( $book_nid , $book_pages = array () ) {
  
  $map = $pages = array ( ) ;
  
  foreach ( $book_pages as $book_page ) $map[$book_page] = $book_page ;
  
  if ( $nodes = book_pages_by_book_nid ( $book_nid ) ) {
  
    foreach ( $nodes as $node ) {

      // Wrap it with Entity API
      $book_page_ewrapper = entity_metadata_wrapper('node', $node);
    
      $sequence_number = $book_page_ewrapper->field_sequence_number->value();
      
      if ( array_key_exists ( $sequence_number, $map ) ) $pages[$sequence_number] = $node ;
      
    }
    
  }
  
  return $pages ;
  
}
