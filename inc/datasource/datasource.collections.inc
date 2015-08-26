<?php

function collections_identifiers ( ) {

  $query = new EntityFieldQuery;

  $result = $query
    ->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'dlts_collection')
    ->execute();

  if (empty($result['node'])) return FALSE;

  $keys = array_keys($result['node']);

  return $keys;

}

function collection_nid ( $identifier ) {

  $query = new EntityFieldQuery;

  $result = $query
    ->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'dlts_collection')
    ->fieldCondition('field_identifier', 'value', $identifier, '=')
    ->execute();

  if (empty($result['node'])) return FALSE;

  $keys = array_keys($result['node']);

  return (int)$keys[0];

}

function collection_title_by_identifier($identifier) {

  $query = new EntityFieldQuery;

  $result = $query
    ->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'dlts_collection')
    ->fieldCondition('field_identifier', 'value', $identifier, '=')
    ->execute();

  if (empty($result['node'])) return FALSE;

  $keys = array_keys($result['node']);

  $node = node_load((int)$keys[0]);

  if ($node) {
    return $node->title ;
  }

}

function collection_books ( $identifier ) {

  $nid = collection_nid ( $identifier ) ;

  $query = new EntityFieldQuery;

  $result = $query
    ->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'dlts_book')
    ->fieldCondition('field_collection', 'nid', $nid, '=')
    ->execute();

  if (empty($result['node'])) return FALSE;

  $keys = array_keys($result['node']);

  return $keys;

}

function collection_maps ( $identifier ) {

  $nid = collection_nid ( $identifier ) ;

  $query = new EntityFieldQuery;

  $result = $query
    ->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'dlts_map')
    ->fieldCondition('field_collection', 'nid', $nid, '=')
    ->execute();

  if (empty($result['node'])) return FALSE;

  $keys = array_keys($result['node']);

  return $keys;

}

function collection_books_published($identifier) {

  $nid = collection_nid($identifier);

  $query = new EntityFieldQuery;

  $result = $query
    ->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'dlts_book')
    ->propertyCondition('status', NODE_PUBLISHED)
    ->fieldCondition('field_collection', 'nid', $nid, '=')
    ->execute();

  if (empty($result['node'])) return FALSE;

  $keys = array_keys($result['node']);

  return $keys;

}

function collection_maps_published($identifier) {

  $nid = collection_nid($identifier);

  $query = new EntityFieldQuery;

  $result = $query
    ->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'dlts_map')
    ->propertyCondition('status', NODE_PUBLISHED)
    ->fieldCondition('field_collection', 'nid', $nid, '=')
    ->execute();

  if (empty($result['node'])) return FALSE;

  $keys = array_keys($result['node']);

  return $keys;

}
