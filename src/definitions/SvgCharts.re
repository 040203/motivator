module ProgressCircle = {
  [@bs.module "react-native-svg-charts"] [@react.component]
  external make:
    (
      ~progress: float,
      ~style: ReactNative.Style.t=?,
      ~progressColor: string=?,
      ~backgroundColor: string=?,
      ~startAngle: float=?,
      ~endAngle: float=?,
      ~strokeWidth: int=?,
      ~cornerRadius: int=?,
      ~animate: bool=?,
      ~animationDuration: int=?,
      ~children: React.element
    ) =>
    React.element =
    "ProgressCircle";
};