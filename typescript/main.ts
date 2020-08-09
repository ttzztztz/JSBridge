// ... app library ...
const Rabbit = {
  Stdin(message: string, cb?: (args: { data: string }) => void) {
    invoke('Stdin', {
      message
    }, cb);
  },

  StdinPromised(message: string) {
    return new Promise(res => {
      Rabbit.Stdin(message, (data) => {
        res(data);
      });
    });
  }
};

// ... developer codes ...

console.log(RabbitJSBridge.invoke.toString());
Rabbit.Stdin('Input something...', ({ data }) => {
  console.log('Get response!');
  console.log(data);
});
