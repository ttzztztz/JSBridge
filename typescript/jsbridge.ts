interface IRabbitJSBridge {
  invoke: (method: string, args: any, callbackId?: number) => any;
}

declare const RabbitJSBridge: IRabbitJSBridge;

type CallBackType = (data: any) => void;

let callBackIncrementId: number = 0;
const callbackMap: {
  [key: number]: CallBackType | undefined
} = {};

function invoke(method: string, args: any, callback?: CallBackType) {
  let currentCallbackId : undefined | number = undefined;
  if (callback) {
    currentCallbackId = ++callBackIncrementId
    callbackMap[currentCallbackId] = callback
  }
  return RabbitJSBridge.invoke(method, args, currentCallbackId);  
}

function RabbitBridgeCallback(cbId: number, data: any) {
  const cb = callbackMap[cbId];
  if (cb) {
    delete callbackMap[cbId]; // in case of memory leak
    cb(data);
  }
}
