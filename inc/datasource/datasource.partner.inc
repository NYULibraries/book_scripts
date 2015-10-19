<?php

function partner_nid ( $identifier ) {

	$query = new EntityFieldQuery ;

	$result = $query
	  ->entityCondition('entity_type', 'node')
	  ->entityCondition('bundle', 'dlts_partner')
	  ->fieldCondition('field_identifier', 'value', $identifier, '=')
	  ->execute() ;

	if ( empty ( $result['node'] ) ) return FALSE ;

	$keys = array_keys ( $result['node'] ) ;

	return (int)$keys[0];

}

function partner_by_identifier ( $identifier ) { }

function partners_books ( $identifier ) { }

function published_books_by_partner_identifier ( $identifier ) {

  $nid = partner_nid ( $identifier ) ;

  $query = new EntityFieldQuery ;

  $result = $query
    ->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'dlts_book')
    ->propertyCondition('status', NODE_PUBLISHED)
    ->fieldCondition('field_partner', 'nid', $nid, '=')
    ->execute() ;

  if ( empty ( $result['node'] ) ) return FALSE ;

  $keys = array_keys ( $result['node'] ) ;

  return $keys ;

}
