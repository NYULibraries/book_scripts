<?php

function get_series_book () {

  $query = new EntityFieldQuery;
  
  $result = $query
    ->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'dlts_series_book')
    ->propertyCondition('status', NODE_PUBLISHED)
    ->execute() ;
    
  if ( empty ( $result['node'] ) ) return array ( ) ;
  
  return array_keys ( $result['node'] ) ;

}

