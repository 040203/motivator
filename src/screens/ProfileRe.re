open ReactNative;
let str = React.string;

[@react.component]
let make = (~navigate) => {
  <> <Text> {str("wtf")} </Text> </>;
};