// ... app library ...
const Rabbit = {
  StdinSync(message: string) : { data: string; } {
    return invoke('StdinSync', {
      message
    });
  }
};

// ... developer codes ...

const { data } = Rabbit.StdinSync("Input something...")
console.log('Get Response!');
console.log(data);
// Rabbit.Stdin('Input something...', ({ data }) => {
//   console.log('Get response!');
//   console.log(data);
// });
