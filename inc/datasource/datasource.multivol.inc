<?php

function multivol_nid ( $identifier ) {

  $query = new EntityFieldQuery;
  
  $result = $query
    ->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'dlts_multivol')
    ->fieldCondition('field_identifier', 'value', $identifier, '=')
    ->execute();
    
  if (empty($result['node'])) return FALSE;

  $keys = array_keys($result['node']);

  return (int)$keys[0];

}

function multivol_book_nid ($identifier, $volume_page ) {

  $query = new EntityFieldQuery;
  
  $result = $query
    ->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'dlts_multivol')
    ->fieldCondition('field_identifier', 'value', $identifier, '=')
	->fieldCondition('field_volume_number', 'value', $volume_page, '=')
    ->execute();
    
  if (empty($result['node'])) return FALSE;

  $keys = array_keys($result['node']);

  return (int)$keys[0];

}

function multivol_book_by_book_nid ( $nid ) {

  $query = new EntityFieldQuery;
  
  $result = $query
    ->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'dlts_multivol_book')
    ->fieldCondition('field_book', 'nid', $nid, '=')
    ->execute();
    
  if (empty($result['node'])) return FALSE;

  $keys = array_keys($result['node']);

  return (int)$keys[0];

}
