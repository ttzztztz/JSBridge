interface IRabbitJSBridge {
  invoke: (method: string, args: any, callbackId?: number) => any;
}

declare const RabbitJSBridge: IRabbitJSBridge;

type CallBackType = (args: any) => void;

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

const subscribes: {
  [key: string] : {
    id: number;
    callback: CallBackType;
  }[] | undefined;
} = {};

function subscribe(method: string, cb: CallBackType) {
  const currentId = ++callBackIncrementId;
  
  const obj = {
    id: currentId,
    callback: cb
  }

  if (subscribes[method]) {
    subscribes[method]!.push(obj);
  } else {
    subscribes[method] = [obj];
  }

  const unsubscribe = () => {
    if (subscribes[method]) {
      subscribes[method] = subscribes[method]?.filter(item => item.id !== currentId);
    }
  }
  return unsubscribe;
}

function triggerEvent(method: string, data: any) {
  subscribes[method]?.forEach(cb => cb.callback(data));
}
