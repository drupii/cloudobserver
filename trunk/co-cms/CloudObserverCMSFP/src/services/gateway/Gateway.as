/**
 * This is a generated sub-class of _Gateway.as and is intended for behavior
 * customization.  This class is only generated when there is no file already present
 * at its target location.  Thus custom behavior that you add here will survive regeneration
 * of the super-class. 
 **/
 
package services.gateway
{
	import com.adobe.fiber.core.model_internal;
	import com.adobe.fiber.services.wrapper.WebServiceWrapper;
	import com.adobe.serializers.utility.TypeUtility;
	import mx.collections.ArrayCollection;
	import mx.rpc.AbstractOperation;
	import mx.rpc.AsyncToken;
	import mx.rpc.soap.mxml.Operation;
	import mx.rpc.soap.mxml.WebService;
public class Gateway extends WebServiceWrapper
{
	/**
	 * This is a generated sub-class of _WorkBlock.as and is intended for behavior
	 * customization.  This class is only generated when there is no file already present
	 * at its target location.  Thus custom behavior that you add here will survive regeneration
	 * of the super-class. 
	 **/
			// Constructor
			public function Gateway()
			{
			}
			public function serviceStart(serviceURL:String):void
			{
				// initialize service control
				_serviceControl = new mx.rpc.soap.mxml.WebService();
				var operations:Object = new Object();
				var operation:mx.rpc.soap.mxml.Operation;    
				
				operation = new mx.rpc.soap.mxml.Operation(null, "IWannaRead");
				operation.resultType = String; 		 
				operations["IWannaRead"] = operation;
				
				operation = new mx.rpc.soap.mxml.Operation(null, "IWannaWrite");
				operation.resultType = String; 		 
				operations["IWannaWrite"] = operation;
				
				_serviceControl.operations = operations;
				try
				{
					_serviceControl.convertResultHandler = com.adobe.serializers.utility.TypeUtility.convertResultHandler;
				}
				catch (e: Error)
				{ /* Flex 3.4 and eralier does not support the convertResultHandler functionality. */ }
				
				_serviceControl.service = "Gateway";
				_serviceControl.port = "BasicHttpBinding_IGateway";         
		
				_serviceControl.wsdl = serviceURL;
				_serviceControl.loadWSDL();			
			}       
			public function IWannaRead(contentIds:Array) : mx.rpc.AsyncToken
			{
				var _internal_operation:mx.rpc.AbstractOperation = _serviceControl.getOperation("IWannaRead");
				var _internal_token:mx.rpc.AsyncToken = _internal_operation.send(contentIds) ;
				return _internal_token;
			}   
			public function IWannaWrite(contentId:int) : mx.rpc.AsyncToken
			{
				model_internal::loadWSDLIfNecessary();
				var _internal_operation:mx.rpc.AbstractOperation = _serviceControl.getOperation("IWannaWrite");
				var _internal_token:mx.rpc.AsyncToken = _internal_operation.send(contentId) ;
				
				return _internal_token;
			}  
			
			
		}
		
	}
