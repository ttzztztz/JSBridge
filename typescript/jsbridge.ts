interface IRabbitJSBridge {
  invoke: (method: string, args: any, callbackId?: number) => void;
}

declare const RabbitJSBridge: IRabbitJSBridge;

type CallBackType = (data: any) => void;

let callBackIncrementId: number = 0;
const callbackMap: {
  [key: number]: CallBackType | undefined
} = {};

export const invoke = (method: string, args: any, callback?: CallBackType) => {
  let currentCallbackId : undefined | number = undefined;
  if (callback) {
    currentCallbackId = ++callBackIncrementId
    callbackMap[currentCallbackId] = callback
  }
  RabbitJSBridge.invoke(method, args, currentCallbackId);  
}

export const callback = (cbId: number, data: any) => {
  const cb = callbackMap[cbId];
  if (cb) {
    cb(data);
    delete callbackMap[cbId]; // in case of memory leak
  }
}
