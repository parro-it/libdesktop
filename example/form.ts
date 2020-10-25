import test from 'tape-async'
import {App,Window,Label,Textfield,Container,YogaFlexDirection} from '../index'
const app = App.create()


const win = new Window({title:"Running on " + process.platform, style: {padding: {left:26,right:26,top:12,bottom:12}}},[   
    new Container({style: {flexDirection: YogaFlexDirection.Row}},[
        new Label({style: {margin:{left:0,top:0}},label: "Given Name"},[]),
        new Textfield({style: {flexGrow:1,margin:{left:12,top:0}},text: "Andrea"},[]),
    ]),
    new Container({style: {flexGrow:1, flexDirection: YogaFlexDirection.Row}},[
        new Label({style: {margin:{left:0,top: 12}},label: "Family Name"},[]),
        new Textfield({style: {flexGrow:1, margin:{left: 12,top: 12}},text: "Parodi"},[]),
    ]),

    new Container({style: {flexDirection: YogaFlexDirection.Row}},[
        new Label({style: {margin:{left:0,top: 12}},label: "City"},[]),
        new Textfield({style: {flexGrow:1,margin:{left: 12,top: 12}},text: "Genoa"},[]),
    ]),
]);

app.start();