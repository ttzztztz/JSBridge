// ... lib codes ...

const Rabbit = {
    StdinSync(message: string) : { data: string; } {
      return invoke('StdinSync', {
        message
      });
    },
    Stdin(message: string, cb: (args: {data: string}) => void) {
      return invoke('Stdin', {
        message
      }, cb);
    },
    ReadFileSync(fileName: string) : { data: ArrayBuffer } {
        return invoke('ReadFileSync', {
            path: fileName
        });
    }
  };

subscribe("onLaunch", ({ timestamp }) => {
  console.warn(`Launched at ${timestamp}`);
});