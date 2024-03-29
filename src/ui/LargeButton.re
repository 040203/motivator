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
          ~marginLeft=auto,
          ~marginRight=auto,
          (),
        ),
    }
  );

[@react.component]
let make =
    (
      ~onPress,
      ~style as customStyle=?,
      ~color=Styles.theme.palette.primary,
      ~title:string=?,
      ~i18n:string=?
    ) => {
  <TouchableHighlight
    underlayColor={Styles.theme.palette.primaryDark}
    style={Style.array([|
      styles##touchable,
      style(~backgroundColor=color, ()),
      Helpers.customStyleFromOption(customStyle),
    |])}
    onPress>
    <StyledText variant=Title i18n value=title color="white" />
  </TouchableHighlight>;
};