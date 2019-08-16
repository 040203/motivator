open ReactNative.Style;

type palette = {
  primary: string,
  secondary: string,
  grey: string,
};

type themeType = {
  palette,
  spacing: float => size,
};

let theme = {
  palette: {
    primary: "#673ab7",
    secondary: "frweef",
    grey: "#bdbdbd",
  },
  spacing: dpValue => dp(dpValue *. 8.),
};

let makeStyles = styleCreateFn =>
  ReactNative.StyleSheet.create(styleCreateFn(theme));