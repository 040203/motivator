open ReactNative;
open ReactNative.Style;

type variant =
  | LargeTitle
  | Title
  | BuildFromProps;

let styles =
  Styles.makeStyles(theme =>
    {
      "title":
        style(
          ~fontSize=20.,
          ~textAlign=`center,
          ~marginBottom=theme.spacing(0.5),
          (),
        ),
      "largeTitle":
        style(
          ~textAlign=`center,
          ~fontSize=36.,
          ~marginTop=theme.spacing(3.),
          ~marginBottom=theme.spacing(1.),
          ~fontWeight=`bold,
          (),
        ),
    }
  );

[@react.component]
let make =
    (
      ~value,
      ~textAlign=`auto,
      ~variant: variant=BuildFromProps,
      ~size=14.,
      ~color="black",
      ~weight=`normal,
      ~style as customStyle=style(),
    ) => {
  let variantStyle =
    switch (variant) {
    | Title => styles##title
    | LargeTitle => styles##largeTitle
    | BuildFromProps => style()
    };

  <Text
    style={Style.array([|
      style(~textAlign, ~fontSize=size, ~fontWeight=weight, ~color, ()),
      variantStyle,
      customStyle,
    |])}>
    {React.string(value)}
  </Text>;
};