open Expo;
open ReactNative;
open ReactNative.Style;

let str = React.string;

let styles =
  Styles.makeStyles(theme =>
    {
      "progress":
        style(
          ~marginTop=theme.spacing(2.),
          ~marginBottom=theme.spacing(7.),
          ~height=200.->dp,
          ~width=200.->dp,
          (),
        ),
      "progressLabelContainer":
        style(
          ~width=200.->dp,
          ~justifyContent=`center,
          ~alignItems=`center,
          (),
        ),
      "progressLabel": style(~fontSize=32., ()),
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
      "addWeight":
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
let make = () => {
  let (currentWeight, setCurrentWight) = React.useState(() => 0.);
  let (isAddRecordOpen, setIsAddRecordOpen) = React.useState(() => false);

  React.useEffect0(() => {
    Db.getLatestWieght(weight => setCurrentWight(_ => weight |> Db.weightGet));
    None;
  });

  let toggleModal = _ => setIsAddRecordOpen(value => !value);

  let handleWeightChange = newWieght => {
    setCurrentWight(_ => newWieght);
    toggleModal();

    (Js.Date.make(), newWieght) |> Db.insertWieght(_ => ());
  };

  <>
    <SvgCharts.ProgressCircle
      style=styles##progress
      progress=0.2
      strokeWidth=10
      progressColor="rgb(134, 65, 244)"
      animate=true
      animationDuration=300>
      <View style=styles##progressLabelContainer>
        <Text style=styles##progressLabel>
          {str(currentWeight->Js.Float.toString ++ "kg")}
        </Text>
      </View>
    </SvgCharts.ProgressCircle>
    <StyledText
      variant=Title
      value={js|До цели осталось 30кг, вперед!|js}
    />
    <TouchableHighlight style=styles##highlight onPress=toggleModal>
      <LinearGradient
        start=[|0., 1.|]
        _end=[|1., 0.|]
        colors=[|
          Styles.theme.palette.primaryLight,
          Styles.theme.palette.primaryDark,
        |]
        style=styles##addWeight>
        <Icons.FontAwesome5 name="plus" size=32 color="white" />
        <StyledText
          color="white"
          size=20.
          value={js|Новое измерение веса|js}
        />
      </LinearGradient>
    </TouchableHighlight>

    {currentWeight !== 0.
       ? <WeightAddRecord
           initialWieght=currentWeight
           isOpen=isAddRecordOpen
           onClose=toggleModal
           onChange=handleWeightChange
         />
       : React.null}
  </>;
};