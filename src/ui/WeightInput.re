open ReactNative;

let options: array(float) = [||];

for (x in 40 to 150) {
  let value = float_of_int(x);
  options
  |> Js.Array.pushMany([|
       value,
       value +. 0.2,
       value +. 0.4,
       value +. 0.6,
       value +. 0.8,
     |])
  |> ignore;
};

[@react.component]
let make = (~value: float, ~style=?, ~onChange: float => unit) => {
  let pickerStyle =
    switch (style) {
    | Some(style) => style
    | None => Style.style(())
    };

  <Picker
    style=pickerStyle
    selectedValue=value
    onValueChange={(value, _) => onChange(value)}>
    {options
     |> Js.Array.map(item =>
          <Picker.Item
            value=item
            key={item |> Js.Float.toString}
            label={item |> Js.Float.toString}
          />
        )
     |> ReasonReact.array}
  </Picker>;
};