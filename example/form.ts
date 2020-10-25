import {App,Window,Label,Textfield,Container,YogaFlexDirection} from '../index'
const app = App.create()

const lbl = new Label({style: {margin:{left:0,top:0}, minWidth:120},text: "Given Name"},[]);
const fld = new Textfield({style: {flexGrow:1,margin:{left:12,top:0}},text: "Andrea"},[])

fld.events.click.listen(() => {
    console.log("THE LABEL HAS BEEN CLICKED");
});


const win = new Window({title:"Running on " + process.platform, style: {padding: {left:26,right:26,top:12,bottom:12}}},[   
    new Container({style: {flexDirection: YogaFlexDirection.Row}},[
        lbl,
        fld,
    ]),
    new Container({style: {flexGrow:1, flexDirection: YogaFlexDirection.Row}},[
        new Label({style: {margin:{left:0,top: 12}, minWidth:120},text: "Family Name"},[]),
        new Textfield({style: {flexGrow:1, margin:{left: 12,top: 12}},text: "Parodi"},[]),
    ]),

    new Container({style: {flexDirection: YogaFlexDirection.Row}},[
        new Label({style: {margin:{left:0,top: 12}, minWidth:120},text: "City"},[]),
        new Textfield({style: {flexGrow:1,margin:{left: 12,top: 12}},text: "Genoa"},[]),
    ]),
]);

app.start();