open ReactNative;
open ReactNative.Style;

let styles =
  Styles.makeStyles(theme =>
    {
      "container": style(~flex=1., ~paddingBottom=theme.spacing(2.), ()),
      "picker": style(),
      "animation": style(~position=`relative, ~flex=0.6, ()),
      "title": style(~marginTop=dp(0.), ()),
      "submitBtn":
        style(~marginTop=auto, ~marginBottom=theme.spacing(6.), ()),
    }
  );

let animations = [|
  Packager.require("../../../../assets/animations/weight.json"),
  Packager.require("../../../../assets/animations/muscle.json"),
  Packager.require("../../../../assets/animations/cycle.json"),
|];

[@react.component]
let make = (~initialWieght, ~isOpen, ~onChange, ~onClose) => {
  let (weight, setWeight) = React.useState(_ => initialWieght);
  let animationRef = React.useRef(Helpers.getRandomItem(animations));

  <Modal animationType=`slide visible=isOpen>
    <SafeAreaView style=styles##container>
      <View style=styles##animation>
        <Lottie autoPlay=true source={animationRef->React.Ref.current} />
      </View>
      <StyledText
        variant=LargeTitle
        textAlign=`center
        style=styles##title
        value="Add weight"
      />
      <StyledText textAlign=`center value="Make sure you have samew clothes" />
      <View style=styles##picker>
        <WeightInput value=weight onChange={value => setWeight(_ => value)} />
      </View>
      <View style=styles##submitBtn>
        <LargeButton onPress={_ => onChange(weight)} title="Submit" />
        <Button title="Close" onPress={_ => onClose()} />
      </View>
    </SafeAreaView>
  </Modal>;
};