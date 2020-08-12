// ... developer codes ...

// sync
const { data: data1 } = Rabbit.StdinSync("(sync1) Input something...");
console.log("(sync1) Get Response!");
console.log(data1);
const { data: data2 } = Rabbit.StdinSync("(sync2) Input something...");
console.log("(sync2) Get Response!");
console.log(data2);

// async
Rabbit.Stdin("(async1) Input something...", ({ data }) => {
  console.log("(async1) Get Response!");
  console.log(data);
});

Rabbit.Stdin("(async2) Input something...", ({ data }) => {
  console.log("(async2) Get Response!");
  console.log(data);
});
