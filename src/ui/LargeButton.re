open ReactNative;
open ReactNative.Style;

let styles =
  Styles.makeStyles(theme =>
    {
      "touchable":
        style(
          ~width=pct(90.),
          ~marginTop=theme.spacing(2.),
          ~marginBottom=theme.spacing(1.),
          ~height=theme.spacing(6.5),
          ~alignItems=`center,
          ~justifyContent=`center,
          ~borderRadius=8.,
          ~backgroundColor=theme.palette.primary,
          ~marginLeft=auto,
          ~marginRight=auto,
          (),
        ),
    }
  );

[@react.component]
let make = (~onPress, ~title) => {
  <TouchableHighlight
    underlayColor={Styles.theme.palette.primaryDark}
    style=styles##touchable
    onPress>
    <StyledText variant=Title value=title color="white" />
  </TouchableHighlight>;
};