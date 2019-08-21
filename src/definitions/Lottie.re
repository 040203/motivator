[@bs.module "lottie-react-native"] [@react.component]
external make:
  (
    ~source: ReactNative.Packager.required,
    ~style: ReactNative.Style.t=?,
    ~autoPlay: bool=?,
    ~loop: bool=?
  ) =>
  React.element =
  "default";