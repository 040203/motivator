open Expo;
open ReactNative;
open ReactNative.Style;

let styles =
  Styles.makeStyles(theme =>
    {
      "highlight":
        style(
          ~marginTop=theme.spacing(2.),
          ~height=theme.spacing(12.),
          ~width=pct(80.),
          ~backgroundColor="#0000",
          ~shadowColor=theme.palette.grey,
          ~shadowRadius=8.,
          ~shadowOffset=offset(~height=8., ~width=0.),
          ~shadowOpacity=0.9,
          ~elevation=12.,
          ~borderRadius=12.,
          (),
        ),
      "gradient":
        style(
          ~height=pct(100.),
          ~width=pct(100.),
          ~padding=theme.spacing(2.),
          ~borderRadius=12.,
          ~justifyContent=`spaceBetween,
          // ~alignItems=`center,
          (),
        ),
    }
  );

[@react.component]
let make = (~icon,~i18n, ~onPress, ~colors) => {
  <TouchableHighlight style=styles##highlight onPress>
    <LinearGradient
      start=[|0., 1.|] _end=[|1., 0.|] colors style=styles##gradient>
      <Icons.FontAwesome5 name=icon size=24 color="white" />
      <StyledText color="white" size=20. i18n=i18n />
    </LinearGradient>
  </TouchableHighlight>;
};