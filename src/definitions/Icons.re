module Ionicons = {
  [@bs.module "@expo/vector-icons"] [@react.component]
  external make: (~name: string, ~color: string=?, ~size: int=?) => React.element = "Ionicons";
};

module FontAwesome5 = {
  [@bs.module "@expo/vector-icons"] [@react.component]
  external make: (~name: string, ~color: string=?, ~size: int=?) => React.element = "FontAwesome5";
};