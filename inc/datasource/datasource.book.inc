<?php

function book_nid ( $identifier ) {

  $query = new EntityFieldQuery;
  
  $result = $query
    ->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'dlts_book')
    ->fieldCondition('field_identifier', 'value', $identifier, '=')
    ->execute();
    
  if (empty($result['node'])) return FALSE;

  $keys = array_keys($result['node']);

  return (int)$keys[0];

}

function book_by_identifier ( $identifier ) {

  $query = new EntityFieldQuery;
  
  $result = $query
    ->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'dlts_book')
    ->propertyCondition('language' , 'en' , '=') 
    ->fieldCondition('field_identifier', 'value', $identifier, '=')
    ->execute();
    
  if ( empty ( $result['node'] ) ) return FALSE ;
  
  $keys = array_keys ( $result['node'] ) ;
  
  return node_load ( $keys[0]  ) ;

}

function books ( ) {

  $query = new EntityFieldQuery;
  
  $result = $query->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'dlts_book')
    ->execute() ;
    
  if ( empty ( $result['node'] ) ) return array ( ) ;

  return array_keys ( $result['node'] ) ;
  
}
