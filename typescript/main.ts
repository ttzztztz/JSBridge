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
Rabbit.Stdin('Input something...', ({ data }) => {
  console.log('Get response! ', data)
});
