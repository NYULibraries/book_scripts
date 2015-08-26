<?php

function get_series () {

  $query = new EntityFieldQuery;
  
  $result = $query
    ->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'dlts_series')
    ->propertyCondition('status', NODE_PUBLISHED)
    ->execute() ;
    
  if ( empty ( $result['node'] ) ) return array ( ) ;
  
  return array_keys ( $result['node'] ) ;

}

