/*********************************************************************************************************************
 * Copyright 2013-2014 Tobii Technology AB. All rights reserved.
 * EyeXProperty.h
 *********************************************************************************************************************/

#if !defined(__TOBII_TX_PROPERTY_API__H__)
#define __TOBII_TX_PROPERTY_API__H__

/*********************************************************************************************************************/

#ifdef TOBII_TX_DETAIL

/*********************************************************************************************************************/

/**
  txCreatePropertyBag

  Creates a property bag.
 
  @param hContext [in]: 
    A TX_CONTEXTHANDLE to the context on which to create the property bag.
    Must not be TX_EMPTY_HANDLE.
  
  @param phBag [out]: 
    A pointer to a TX_HANDLE which will be set to the newly created property bag.
    This handle must be released using txReleaseObject to avoid leaks.
    Must not be NULL.
    The value of the pointer must be set to TX_EMPTY_HANDLE.
  
  @param type [in]: 
    A TX_PROPERTYBAGTYPE which specifies what type of property bag to create.
 
  @return 
    TX_RESULT_OK: The property bag was successfully created.
    TX_RESULT_SYSTEMNOTINITIALIZED: The system is not initialized.
    TX_RESULT_INVALIDARGUMENT: An invalid argument was passed to the function.    
 */
TX_API_FUNCTION(CreatePropertyBag,(
    TX_CONTEXTHANDLE hContext,
    TX_OUT_PARAM(TX_HANDLE) phBag,
    TX_PROPERTYBAGTYPE type
    ));

/*********************************************************************************************************************/

/**
  txGetPropertyBagType

  Gets the TX_PROPERTYBAGTYPE of a property bag.
 
  @param hBounds [in]: 
    A TX_CONSTHANDLE to the property bag.
    Must not be TX_EMPTY_HANDLE.
 
  @param pBoundsType [out]: 
    A pointer to a TX_PROPERTYBAGTYPE which will be set to the type of the property bag.
    Must not be NULL.
 
  @return 
    TX_RESULT_OK: The type of the property bag was successfully retrieved.
    TX_RESULT_SYSTEMNOTINITIALIZED: The system is not initialized.
    TX_RESULT_INVALIDARGUMENT: An invalid argument was passed to the function.
 */
TX_API_FUNCTION(GetPropertyBagType,(
    TX_CONSTHANDLE hBag,
    TX_OUT_PARAM(TX_PROPERTYBAGTYPE) pType
    ));

/*********************************************************************************************************************/

/**
  txCreateProperty

  Creates a property on an interaction object.
 
  @param hObject [in]: 
    A TX_HANDLE to the object on which to create the property.
    Must not be TX_EMPTY_HANDLE.
  
  @param phProperty [out]: 
    A pointer to a TX_PROPERTYHANDLE which will be set to the newly created property. 
    Must not be NULL.
    The value of the pointer must be set to TX_EMPTY_HANDLE.
  
  @param propertyName [in]: 
    The name of the property.
    Can be NULL or empty only if the object is of type TX_INTERACTIONOBJECTTYPE_PROPERTYBAG with bag type 
    TX_PROPERTYBAGTYPE_ARRAY.
 
  @return 
    TX_RESULT_OK: The property was successfully created.
    TX_RESULT_SYSTEMNOTINITIALIZED: The system is not initialized.
    TX_RESULT_INVALIDARGUMENT: An invalid argument was passed to the function.
    TX_RESULT_INVALIDPROPERTYNAME: The name of the property was invalid.
    TX_RESULT_DUPLICATEPROPERTY: There already exists a property with the specified name on this object.
 */
TX_API_FUNCTION(CreateProperty,(
    TX_HANDLE hObject,
    TX_OUT_PARAM(TX_PROPERTYHANDLE) phProperty,
    TX_CONSTSTRING propertyName
    ));

/*********************************************************************************************************************/

/**
  txRemoveProperty

  Removes a property from an interaction object.
 
  @param hObject [in]: 
    A TX_HANDLE to the object from which to remove the property.
    Must not be TX_EMPTY_HANDLE.
  
  @param propertyName [in]: 
    The name of the property to remove.
    Must not be NULL or empty string.
 
  @return 
    TX_RESULT_OK: The property was successfully removed.
    TX_RESULT_SYSTEMNOTINITIALIZED: The system is not initialized.
    TX_RESULT_INVALIDARGUMENT: An invalid argument was passed to the function.
    TX_RESULT_NOTFOUND: A property with the specified name was not found. 
    TX_RESULT_PROPERTYNOTREMOVABLE: The specified property can not be removed.
 */
TX_API_FUNCTION(RemoveProperty,(
    TX_HANDLE hObject,
    TX_CONSTSTRING propertyName
    ));

/*********************************************************************************************************************/

/**
  txGetProperty

  Gets a property from an interaction object.
 
  @param hObject [in]: 
    A TX_CONSTHANDLE to the object from which to get the property.
    Must not be TX_EMPTY_HANDLE.
  
  @param phProperty [out]: 
    A pointer to a TX_PROPERTYHANDLE which will be set to the property.
    Must not be NULL.
    The value of the pointer must be set to TX_EMPTY_HANDLE.
  
  @param propertyName [in]: 
    The name of the property to get. 
    Must not be NULL or empty string.
 
  @return 
    TX_RESULT_OK: The property was successfully created.
    TX_RESULT_SYSTEMNOTINITIALIZED: The system is not initialized.
    TX_RESULT_INVALIDARGUMENT: An invalid argument was passed to the function.
    TX_RESULT_INVALIDPROPERTYNAME: The name of the property was invalid.
    TX_RESULT_NOTFOUND: A property with the specified name was not found. 
 */
TX_API_FUNCTION(GetProperty,(
    TX_CONSTHANDLE hObject,
    TX_OUT_PARAM(TX_PROPERTYHANDLE) phProperty,
    TX_CONSTSTRING propertyName
    ));

/*********************************************************************************************************************/

/**
  txGetProperties

  Gets TX_HANDLEs to all properties on an interaction object.
 
  @param hObject [in]: 
    A TX_CONSTHANDLE to the object from which to get the properties.
    Must not be TX_EMPTY_HANDLE.
 
  @param phProperties [out]: 
    A pointer to an array of TX_PROPERTYHANDLEs to which the property handles will be copied.
    Can be NULL to only get the size.
 
  @param pPropertiesSize [in,out]: 
    A pointer to a TX_SIZE which will be set to the number of properties.
    Must not be NULL.
    The value must be 0 if phProperties is NULL.
 
  @return 
    TX_RESULT_OK: The handles or the required buffer size was retrieved successfully.
    TX_RESULT_SYSTEMNOTINITIALIZED: The system is not initialized.
    TX_RESULT_INVALIDARGUMENT: An invalid argument was passed to the function.
    TX_RESULT_INVALIDBUFFERSIZE: The size of the array is invalid. (*pPropertiesSize will be set to the number of behaviors).
 */
TX_API_FUNCTION(GetProperties,(
    TX_CONSTHANDLE hObject,
    TX_PTR_PARAM(TX_PROPERTYHANDLE) phProperties,
    TX_REF_PARAM(TX_SIZE) pPropertiesSize
    ));

/*********************************************************************************************************************/

/**
  txGetPropertyName

  Gets the name of a property.
 
  @param hProperty [in]: 
    A TX_CONSTPROPERTYHANDLE to the property for which to get the name.
    Must not be TX_EMPTY_HANDLE.
 
  @param pName [out]: 
    A TX_STRING to which the property name will be copied.
    Must be at least the size of the property name.
    Can be NULL to only get the size of the property name.
 
  @param pNameSize [in,out]: 
    A pointer to a TX_SIZE which will be set to the size of the property name.
    Must not be NULL.
    The value must be 0 if pName is NULL.
 
  @return 
    TX_RESULT_OK: The property name or the required size of the string was successfully retrieved.
    TX_RESULT_SYSTEMNOTINITIALIZED: The system is not initialized.
    TX_RESULT_INVALIDARGUMENT: An invalid argument was passed to the function.
    TX_RESULT_INVALIDBUFFERSIZE: The size of pName is invalid (pNameSize will be set to the required size).
 */
TX_API_FUNCTION(GetPropertyName,(
    TX_CONSTPROPERTYHANDLE hProperty,    
    TX_STRING pName,
    TX_REF_PARAM(TX_SIZE) pNameSize
    ));

/*********************************************************************************************************************/

/**
  txGetPropertyValueType

  Gets the TX_PROPERTYVALUETYPE of a property.
 
  @param hProperty [in]: 
    A TX_CONSTPROPERTYHANDLE to the property.
    Must not be TX_EMPTY_HANDLE.
 
  @param pPropertyType [out]: 
    A pointer to a TX_PROPERTYVALUETYPE which will be set to the type of the value.
    Must not be NULL.
 
  @return 
    TX_RESULT_OK: The type of the value was successfully retrieved.
    TX_RESULT_SYSTEMNOTINITIALIZED: The system is not initialized.
    TX_RESULT_INVALIDARGUMENT: An invalid argument was passed to the function.
 */
TX_API_FUNCTION(GetPropertyValueType,(
    TX_CONSTPROPERTYHANDLE hProperty,    
    TX_OUT_PARAM(TX_PROPERTYVALUETYPE) pPropertyType
    ));

/*********************************************************************************************************************/

/**
  txGetPropertyFlags

  Gets the TX_PROPERTYFLAGS of a property.
 
  @param hProperty [in]: 
    A TX_CONSTPROPERTYHANDLE to the property.
    Must not be NULL.
 
  @param pFlags [out]: 
    A pointer to a TX_PROPERTYFLAGS which will be set to the flags.
    Must not be NULL.
 
  @return 
    TX_RESULT_OK: The flags were successfully retrieved.
    TX_RESULT_SYSTEMNOTINITIALIZED: The system is not initialized.
    TX_RESULT_INVALIDARGUMENT: An invalid argument was passed to the function.
 */
TX_API_FUNCTION(GetPropertyFlags,(
    TX_CONSTPROPERTYHANDLE hProperty,    
    TX_OUT_PARAM(TX_PROPERTYFLAGS) pFlags
    ));

/*********************************************************************************************************************/

/**
  txClearPropertyValue

  Clears the value of a property.
  The value type will be set to TX_PROPERTYVALUETYPE_EMPTY.
 
  @param hProperty [in]: 
    A TX_PROPERTYHANDLE to the property to clear.
    Must not be TX_EMPTY_HANDLE.
 
  @return 
    TX_RESULT_OK: The property was successfully cleared.
    TX_RESULT_SYSTEMNOTINITIALIZED: The system is not initialized.
    TX_RESULT_INVALIDARGUMENT: An invalid argument was passed to the function.
 */
TX_API_FUNCTION(ClearPropertyValue,(
    TX_PROPERTYHANDLE hProperty
    ));

/*********************************************************************************************************************/

/**
  txSetPropertyValueAsInteger

  Sets the value of a property to a TX_INTEGER.
  If the property already has a value that will be overwritten, regardless of type.
  The value type will be set to TX_PROPERTYVALUETYPE_INTEGER.
 
  @param hProperty [in]: 
    A TX_PROPERTYHANDLE to the property for which the value should be set.
    Must not be TX_EMPTY_HANDLE.
 
  @param intValue [in]: 
    A TX_INTEGER which is the value to set.
 
  @return 
    TX_RESULT_OK: The property value was successfully set.
    TX_RESULT_SYSTEMNOTINITIALIZED: The system is not initialized.
    TX_RESULT_INVALIDARGUMENT: An invalid argument was passed to the function.
 */
TX_API_FUNCTION(SetPropertyValueAsInteger,(
    TX_PROPERTYHANDLE hProperty,    
    TX_INTEGER intValue
    ));

/*********************************************************************************************************************/

/**
  txSetPropertyValueAsReal    

  Sets the value of a property to a TX_REAL.
  If the property already has a value that will be overwritten, regardless of type.
  The value type will be set to TX_PROPERTYVALUETYPE_REAL.
 
  @param hProperty [in]: 
    A TX_PROPERTYHANDLE to the property for which the value should be set.
    Must not be TX_EMPTY_HANDLE.
 
  @param realValue [in]: 
    A TX_REAL which is the value to set.
 
  @return 
    TX_RESULT_OK: The property value was successfully set.
    TX_RESULT_SYSTEMNOTINITIALIZED: The system is not initialized.
    TX_RESULT_INVALIDARGUMENT: An invalid argument was passed to the function.
 */
TX_API_FUNCTION(SetPropertyValueAsReal,(
    TX_PROPERTYHANDLE hProperty,    
    TX_REAL realValue
    ));

/*********************************************************************************************************************/

/**
  txSetPropertyValueAsString    

  Sets the value of a property to a string.
  If the property already has a value that will be overwritten, regardless of type.
  The value type will be set to TX_PROPERTYVALUETYPE_STRING.
 
  @param hProperty [in]: 
    A TX_PROPERTYHANDLE to the property for which the value should be set.
    Must not be TX_EMPTY_HANDLE.
 
  @param stringValue [in]: 
    A TX_CONSTSTRING which is the value to set.
    Must not be NULL.
 
  @return 
    TX_RESULT_OK: The property value was successfully set.
    TX_RESULT_SYSTEMNOTINITIALIZED: The system is not initialized.
    TX_RESULT_INVALIDARGUMENT: An invalid argument was passed to the function.
 */
TX_API_FUNCTION(SetPropertyValueAsString,(
    TX_PROPERTYHANDLE hProperty,    
    TX_CONSTSTRING stringValue
    ));

/*********************************************************************************************************************/

/**
  txSetPropertyValueAsObject    

  Sets the value of a property to an interaction object.
  If the property already has a value that will be overwritten, regardless of type.
  The value type will be set to TX_PROPERTYVALUETYPE_OBJECT.
 
  @param hProperty [in]: 
    A TX_PROPERTYHANDLE to the property for which the value should be set.
    Must not be TX_EMPTY_HANDLE.
 
  @param hObject [in]: 
    A TX_HANDLE to the obejct which is the value to set.
    Must not be NULL.
 
  @return 
    TX_RESULT_OK: The property value was successfully set.
    TX_RESULT_SYSTEMNOTINITIALIZED: The system is not initialized.
    TX_RESULT_INVALIDARGUMENT: An invalid argument was passed to the function.
 */
TX_API_FUNCTION(SetPropertyValueAsObject,(
    TX_PROPERTYHANDLE hProperty,    
    TX_HANDLE hObject
    ));

/*********************************************************************************************************************/

/**
  txSetPropertyValueAsBlob    

  Sets the value of a property to a blob.
  If the property already has a value that will be overwritten, regardless of type.
  The value type will be set to TX_PROPERTYVALUETYPE_BLOB.
 
  @param hProperty [in]: 
    A TX_PROPERTYHANDLE to the property for which the value should be set.
    Must not be TX_EMPTY_HANDLE.
 
  @param pBuffer [in]: 
    A pointer to an array of bytes.
    Must not be NULL.
 
  @param blobSize [in]: 
    A TX_SIZE which specifies the size of the blob (i.e. the number of bytes).
 
  @return 
    TX_RESULT_OK: The property value was successfully set.
    TX_RESULT_SYSTEMNOTINITIALIZED: The system is not initialized.
    TX_RESULT_INVALIDARGUMENT: An invalid argument was passed to the function.
 */
TX_API_FUNCTION(SetPropertyValueAsBlob,(
    TX_PROPERTYHANDLE hProperty,    
    TX_CONSTPTR_PARAM(TX_BYTE) pBuffer,
    TX_SIZE blobSize
    ));

/*********************************************************************************************************************/

/**
  txGetPropertyValueAsInteger    

  Gets the value of a property as a TX_INTEGER.
  If the property does not have a value of this type this call will fail.
 
  @param hProperty [in]: 
    A TX_CONSTPROPERTYHANDLE to the property for which the value should be retrieved.
    Must not be TX_EMPTY_HANDLE.
 
  @param pIntValue [out]: 
    A pointer to a TX_INTEGER which will be set to the value of the property.
 
  @return 
    TX_RESULT_OK: The property value was successfully retrieved.
    TX_RESULT_SYSTEMNOTINITIALIZED: The system is not initialized.
    TX_RESULT_INVALIDARGUMENT: An invalid argument was passed to the function.
    TX_RESULT_INVALIDPROPERTYTYPE: The value type was not TX_PROPERTYVALUETYPE_INTEGER.
 */
TX_API_FUNCTION(GetPropertyValueAsInteger,(
    TX_CONSTPROPERTYHANDLE hProperty,    
    TX_OUT_PARAM(TX_INTEGER) pIntValue
    ));

/*********************************************************************************************************************/

/**
  txGetPropertyValueAsReal    

  Gets the value of a property as a TX_REAL.
  If the property does not have a value or have a value of another type this call will fail.
 
  @param hProperty [in]: 
    A TX_CONSTPROPERTYHANDLE to the property for which the value should be retrieved.
    Must not be TX_EMPTY_HANDLE.
 
  @param pRealValue [out]: 
    A pointer to a TX_REAL which will be set to the value of the property.
 
  @return 
    TX_RESULT_OK: The property value was successfully retrieved.
    TX_RESULT_SYSTEMNOTINITIALIZED: The system is not initialized.
    TX_RESULT_INVALIDARGUMENT: An invalid argument was passed to the function.
    TX_RESULT_INVALIDPROPERTYTYPE: The value type was not TX_PROPERTYVALUETYPE_REAL.
 */
TX_API_FUNCTION(GetPropertyValueAsReal,(
    TX_CONSTPROPERTYHANDLE hProperty,    
    TX_OUT_PARAM(TX_REAL) pRealValue
    ));

/*********************************************************************************************************************/

/**
  txGetPropertyValueAsString    

  Gets the value of a property as a string.
  If the property does not have a value or have a value of another type this call will fail.
 
  @param hProperty [in]: 
    A TX_CONSTPROPERTYHANDLE to the property for which the value should be retrieved.
    Must not be TX_EMPTY_HANDLE.
 
  @param pStringValue [out]: 
    A TX_STRING to which the property value will be copied.
    Must be at least the size of the value.
    Can be NULL to only get the size of the value.
 
  @param pStringSize [in,out]: 
    A pointer to a TX_SIZE which will be set to the size of the property value.
    Must not be NULL.
    The value must be 0 if pStringValue is NULL.
 
  @return 
    TX_RESULT_OK: The property value or the required size of the string was successfully retrieved.
    TX_RESULT_SYSTEMNOTINITIALIZED: The system is not initialized.
    TX_RESULT_INVALIDARGUMENT: An invalid argument was passed to the function.
    TX_RESULT_INVALIDBUFFERSIZE: The size of pStringValue is invalid (*pStringSize will be set to the required size). 
    TX_RESULT_INVALIDPROPERTYTYPE: The value type was not TX_PROPERTYVALUETYPE_STRING.
 */
TX_API_FUNCTION(GetPropertyValueAsString,(
    TX_CONSTPROPERTYHANDLE hProperty,    
    TX_STRING pStringValue,
    TX_REF_PARAM(TX_SIZE) pStringSize
    ));

/*********************************************************************************************************************/

/**
  txGetPropertyValueAsObject    

  Gets the value of a property as an interaction object.
  If the property does not have a value or have a value of another type this call will fail.
 
  @param hProperty [in]: 
    A TX_CONSTPROPERTYHANDLE to the property for which the value should be retrieved.
    Must not be TX_EMPTY_HANDLE.
 
  @param phObject [out]: 
    A pointer to a TX_HANDLE which will be set to the value of the property.
    This handle must be released using txReleaseObject to avoid leaks.
    Must not be NULL.
    The value of the pointer must be set to TX_EMPTY_HANDLE.
 
  @return 
    TX_RESULT_OK: The property value was successfully retrieved.
    TX_RESULT_SYSTEMNOTINITIALIZED: The system is not initialized.
    TX_RESULT_INVALIDARGUMENT: An invalid argument was passed to the function.
    TX_RESULT_INVALIDPROPERTYTYPE: The value type was not TX_PROPERTYVALUETYPE_OBJECT.
 */
TX_API_FUNCTION(GetPropertyValueAsObject,(
    TX_CONSTPROPERTYHANDLE hProperty,    
    TX_OUT_PARAM(TX_HANDLE) phObject
    ));

/*********************************************************************************************************************/

/**
  txGetPropertyValueAsBlob    

  Gets the value of a property as a blob.
  If the property does not have a value or have a value of another type this call will fail.
 
  @param hProperty [in]: 
    A TX_CONSTPROPERTYHANDLE to the property for which the value should be retrieved.
    Must not be TX_EMPTY_HANDLE.
 
  @param pBuffer [out]: 
    A pointer to a byte array to which the property value will be copied.
    Must be at least the size of the value (i.e. number of bytes in the blob).
    Can be NULL to only get the size of the blob.
 
  @param pBlobSize [in,out]: 
    A pointer to a TX_SIZE which will be set to the size of the blob.
    Must not be NULL.
    The value must be 0 if pBuffer is NULL.
 
  @return 
    TX_RESULT_OK: The property value or the required size of the buffer was successfully retrieved.
    TX_RESULT_SYSTEMNOTINITIALIZED: The system is not initialized.
    TX_RESULT_INVALIDARGUMENT: An invalid argument was passed to the function.
    TX_RESULT_INVALIDBUFFERSIZE: The size of pBuffer is invalid (*pBlobSize will be set to the required size). 
    TX_RESULT_INVALIDPROPERTYTYPE: The value type was not TX_PROPERTYVALUETYPE_BLOB.
 */
TX_API_FUNCTION(GetPropertyValueAsBlob,(
    TX_CONSTPROPERTYHANDLE hProperty,    
    TX_PTR_PARAM(TX_BYTE) pBuffer,
    TX_REF_PARAM(TX_SIZE) pBlobSize
    ));

/*********************************************************************************************************************/

/**
  txCopyProperties    

  Makes a shallow copy of the properties in the source object to the target object.
 
  @param hSourceObject [in]: 
    A TX_CONSTHANDLE to the source object.
    Must not be TX_EMPTY_HANDLE.
 
  @param hTargetObject [in]: 
    A TX_HANDLE to the target object.
    Must not be TX_EMPTY_HANDLE.
  
  @return 
    TX_RESULT_OK: The properties were successfully copied.
    TX_RESULT_SYSTEMNOTINITIALIZED: The system is not initialized.
    TX_RESULT_INVALIDARGUMENT: An invalid argument was passed to the function. 
 */
TX_API_FUNCTION(CopyProperties,(
    TX_CONSTHANDLE hSourceObject,    
    TX_HANDLE hTargetObject
    ));

/*********************************************************************************************************************/

#endif /* define TOBII_TX_DETAIL */

/*********************************************************************************************************************/

#endif /* !defined(__TOBII_TX_PROPERTY_API__H__) */

/*********************************************************************************************************************/
