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
