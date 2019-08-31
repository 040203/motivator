let getRandomItem = array => array[Js.Math.random_int(0, array->Array.length)];

let customStyleFromOption = style =>
  switch (style) {
  | Some(style) => style
  | None => ReactNative.Style.style()
  };

let makeProviderProps = (~value, ~children, ()) => {
  "value": value,
  "children": children,
};